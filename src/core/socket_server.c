// core/socket_server.c
#include "socket_server.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/un.h>
#include <pthread.h>
#include "page_manager.h"
#include "ui_job_queue.h"
#include "../utils/net_util.h"
#include "../utils/debug.h"
#include "network_vlan.h"

static int server_fd = -1;
static pthread_t server_thread;
static char last_value[10000] = {0};
static char last_vlan_value[256] = {0};
static char last_vlan_ipv6_value[10000] = {0};

static void trim_trailing(char * s)
{
    size_t len;

    if(!s) {
        return;
    }

    len = strlen(s);
    while(len > 0 && (s[len - 1] == '\n' || s[len - 1] == '\r' || s[len - 1] == ' ' || s[len - 1] == '\t')) {
        s[--len] = '\0';
    }
}

static void * ipc_server_thread(void * arg)
{
    const char * socket_path = (const char *)arg;
    struct sockaddr_un addr;
    char buf[10000];

    // craete socket
    if((server_fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        return NULL;
    }

    // bind socket
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, socket_path, sizeof(addr.sun_path) - 1);
    unlink(socket_path); // Ensure socket file does not already exist

    if(bind(server_fd, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
        perror("bind");
        close(server_fd);
        return NULL;
    }

    // listen
    if(listen(server_fd, 5) == -1) {
        perror("listen");
        close(server_fd);
        return NULL;
    }

    while(1) {
        int client_fd;
        struct sockaddr_un client_addr;
        socklen_t len = sizeof(client_addr);

        if((client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &len)) == -1) {
            perror("accept");
            continue;
        }

        ssize_t n = read(client_fd, buf, sizeof(buf) - 1);
        if(n > 0) {
            GlobalData * page_data;
            char response[256] = {0};

            buf[n] = '\0';
            trim_trailing(buf);

            char * key   = strtok(buf, "=");
            char * value = strtok(NULL, "=");
            if(key) {
                trim_trailing(key);
            }
            if(value) {
                trim_trailing(value);
            }

            page_data = page_get_global_data();

            if(key != NULL) {
                if(strcmp(key, "REPORT_IPV6_ADDRESSES") != 0 && strcmp(key, "REPORT_VLAN_IPV6_ADDRESSES") != 0) {
                    DEBUG_LOG("IPC Command Received: %s=%s\n", key, value ? value : "NULL");
                    DEBUG_LOG(" old orientation      = %d\n", page_data->orientation);
                    DEBUG_LOG(" old sleep_time       = %d\n", page_data->sleep_time);
                    DEBUG_LOG(" old screen_luminance = %d\n", page_data->screen_luminance);
                    DEBUG_LOG(" old dark_screen_time = %d\n", page_data->dark_screen_time);
                }

                if(strcmp(key, "GET_CONFIG") == 0) {
                    /* Snapshot GlobalData only — no LVGL from this thread. */
                    ui_job_format_config_response(response, sizeof(response));
                } else if(strcmp(key, "SCREEN_ORIENTATION") == 0 && value != NULL) {
                    /* 1 = LV_DISP_ROT_90, 0 = LV_DISP_ROT_270; apply on UI thread. */
                    ui_job_post_orientation(atoi(value));
                    snprintf(response, sizeof(response), "OK");
                } else if(strcmp(key, "SCREEN_LUMINANCE") == 0 && value != NULL) {
                    ui_job_post_luminance(atoi(value));
                    snprintf(response, sizeof(response), "OK");
                } else if(strcmp(key, "DARK_SCREEN_TIME") == 0 && value != NULL) {
                    ui_job_post_dark_screen_time(atoi(value));
                    snprintf(response, sizeof(response), "OK");
                } else if(strcmp(key, "SLEEP_TIME") == 0 && value != NULL) {
                    ui_job_post_sleep_time(atoi(value));
                    snprintf(response, sizeof(response), "OK");
                } else if(strcmp(key, "REPORT_IPV6_ADDRESSES") == 0 && value != NULL) {
                    snprintf(response, sizeof(response), "OK");
                    if(strcmp(value, last_value) != 0) {
                        char * show_ipv6 = handle_ipv6_report(value);
                        set_tmp_ipv6_address(show_ipv6);
                        DEBUG_LOG("Received new IPv6 report: %s\n", value);
                        if(show_ipv6) {
                            free(show_ipv6);
                        }
                        strncpy(last_value, value, sizeof(last_value) - 1);
                        last_value[sizeof(last_value) - 1] = '\0';
                        page_request_update_all();
                    }
                } else if(strcmp(key, "REPORT_VLAN_CONFIG") == 0 && value != NULL) {
                    snprintf(response, sizeof(response), "OK");
                    if(strcmp(value, last_vlan_value) != 0) {
                        if(network_vlan_apply_report(page_data, value)) {
                            strncpy(last_vlan_value, value, sizeof(last_vlan_value) - 1);
                            last_vlan_value[sizeof(last_vlan_value) - 1] = '\0';
                            page_request_update_all();
                        }
                    }
                } else if(strcmp(key, "REPORT_VLAN_IPV6_ADDRESSES") == 0 && value != NULL) {
                    snprintf(response, sizeof(response), "OK");
                    if(strcmp(value, last_vlan_ipv6_value) != 0) {
                        if(network_vlan_apply_ipv6_report(page_data, value)) {
                            strncpy(last_vlan_ipv6_value, value, sizeof(last_vlan_ipv6_value) - 1);
                            last_vlan_ipv6_value[sizeof(last_vlan_ipv6_value) - 1] = '\0';
                            page_request_update_all();
                        }
                    } else {
                        DEBUG_LOG("REPORT_VLAN_IPV6_ADDRESSES: unchanged, skipped\n");
                    }
                } else {
                    snprintf(response, sizeof(response), "ERROR=Unknown command");
                }
            }

            write(client_fd, response, strlen(response));
        }
        close(client_fd);
    }
    return NULL;
}

void ipc_server_start(const char * socket_path)
{
    pthread_create(&server_thread, NULL, ipc_server_thread, (void *)socket_path);
}

void ipc_server_stop(void)
{
    if(server_fd != -1) {
        close(server_fd);
        server_fd = -1;
    }
    pthread_cancel(server_thread);
}

/* Client: send a command to arkkvm-app cmd socket (/tmp/arkkvm_cmd.sock). */
bool ipc_send_command_timeout(const char * cmd, char * response, size_t resp_size,
                              int timeout_sec)
{
    const char * sock_path = "/tmp/arkkvm_cmd.sock";
    int fd;
    struct sockaddr_un addr;
    ssize_t n;
    struct timeval tv;

    if(!cmd || !response || resp_size == 0) {
        return false;
    }
    if(timeout_sec <= 0) {
        timeout_sec = 5;
    }
    response[0] = '\0';

    fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if(fd < 0) {
        snprintf(response, resp_size, "ERROR=socket");
        return false;
    }

    tv.tv_sec  = timeout_sec;
    tv.tv_usec = 0;
    setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
    setsockopt(fd, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv));

    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, sock_path, sizeof(addr.sun_path) - 1);

    if(connect(fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        snprintf(response, resp_size, "ERROR=connect");
        close(fd);
        return false;
    }

    if(write(fd, cmd, strlen(cmd)) < 0) {
        snprintf(response, resp_size, "ERROR=write");
        close(fd);
        return false;
    }

    n = read(fd, response, resp_size - 1);
    close(fd);
    if(n <= 0) {
        snprintf(response, resp_size, "ERROR=read");
        return false;
    }
    response[n] = '\0';
    trim_trailing(response);
    return strncmp(response, "OK", 2) == 0;
}

bool ipc_send_command(const char * cmd, char * response, size_t resp_size)
{
    return ipc_send_command_timeout(cmd, response, resp_size, 5);
}
