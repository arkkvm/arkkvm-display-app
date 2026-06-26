// core/socket_server.c
#include "socket_server.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <pthread.h>
#include "data_manager.h"
#include "page_manager.h"
#include "config_manager.h"
#include "../utils/command.h"
#include "../utils/net_util.h"
#include "../utils/debug.h"
#include "../utils/time_util.h"
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

        //
        if((client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &len)) == -1) {
            perror("accept");
            continue;
        }

        // read command
        ssize_t n = read(client_fd, buf, sizeof(buf) - 1);
        if(n > 0) {
            buf[n]             = '\0';
            trim_trailing(buf);
            char response[256] = {0};

            // Parse KEY=VALUE
            char * key             = strtok(buf, "=");
            char * value           = strtok(NULL, "=");
            if(key) {
                trim_trailing(key);
            }
            if(value) {
                trim_trailing(value);
            }
            GlobalData * page_data = page_get_global_data();
            bool is_change         = false;
            if(key != NULL) {
                if(strcmp(key, "REPORT_IPV6_ADDRESSES") != 0 && strcmp(key, "REPORT_VLAN_IPV6_ADDRESSES") != 0) {
                    DEBUG_LOG("IPC Command Received: %s=%s\n", key, value ? value : "NULL");
                    DEBUG_LOG(" old orientation      = %d\n", page_data->orientation);
                    DEBUG_LOG(" old sleep_time       = %d\n", page_data->sleep_time);
                    DEBUG_LOG(" old screen_luminance = %d\n", page_data->screen_luminance);
                    DEBUG_LOG(" old dark_screen_time = %d\n", page_data->dark_screen_time);
                }

                if(strcmp(key, "GET_CONFIG") == 0) {
                    // Get all configuration values
                    lv_disp_rot_t current_rotation = lv_disp_get_rotation(lv_disp_get_default());
                    int orientation                = (current_rotation == LV_DISP_ROT_90) ? 1 : 0;
                    int luminance                  = page_data->screen_luminance;
                    int dark_time                  = page_data->dark_screen_time;
                    int sleep_time                 = page_data->sleep_time;

                    snprintf(response, sizeof(response),
                             "ORIENTATION=%d,LUMINANCE=%d,DARK_SCREEN_TIME=%d,SLEEP_TIME=%d", orientation, luminance,
                             dark_time, sleep_time);

                } else if(strcmp(key, "SCREEN_ORIENTATION") == 0 && value != NULL) {

                    // 1 = normal LV_DISP_ROT_90, 0 = inverted LV_DISP_ROT_270
                    // Set screen orientation
                    int orientation = atoi(value);
                    lv_disp_set_rotation(lv_obj_get_disp(lv_scr_act()),
                                         orientation == 1 ? LV_DISP_ROT_90 : LV_DISP_ROT_270);
                    snprintf(response, sizeof(response), "OK");
                    page_data->orientation = orientation;
                    is_change              = true;
                } else if(strcmp(key, "SCREEN_LUMINANCE") == 0 && value != NULL) {
                    // Set screen luminance
                    int luminance = atoi(value);
                    page_data->screen_luminance = luminance;
                    snprintf(response, sizeof(response), "OK");
                    page_data->lastTouchTime = lv_tick_get();
                    update_screen_luminance();
                    is_change = true;
                } else if(strcmp(key, "DARK_SCREEN_TIME") == 0 && value != NULL) {
                    // Set dark screen timeout
                    int sec = atoi(value);
                    page_data->dark_screen_time = sec;
                    snprintf(response, sizeof(response), "OK");
                    is_change = true;
                } else if(strcmp(key, "SLEEP_TIME") == 0 && value != NULL) {
                    // Set sleep timeout
                    int sec = atoi(value);
                    page_data->sleep_time = sec;
                    snprintf(response, sizeof(response), "OK");
                    is_change = true;

                }

                else if(strcmp(key, "REPORT_IPV6_ADDRESSES") == 0 && value != NULL) {
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
                    }
                } else if(strcmp(key, "REPORT_VLAN_CONFIG") == 0 && value != NULL) {
                    snprintf(response, sizeof(response), "OK");
                    if(strcmp(value, last_vlan_value) != 0) {
                        if(network_vlan_apply_report(page_data, value)) {
                            strncpy(last_vlan_value, value, sizeof(last_vlan_value) - 1);
                            last_vlan_value[sizeof(last_vlan_value) - 1] = '\0';
                            page_update_all();
                        }
                    }
                } else if(strcmp(key, "REPORT_VLAN_IPV6_ADDRESSES") == 0 && value != NULL) {
                    snprintf(response, sizeof(response), "OK");
                    if(strcmp(value, last_vlan_ipv6_value) != 0) {
                        if(network_vlan_apply_ipv6_report(page_data, value)) {
                            strncpy(last_vlan_ipv6_value, value, sizeof(last_vlan_ipv6_value) - 1);
                            last_vlan_ipv6_value[sizeof(last_vlan_ipv6_value) - 1] = '\0';
                            page_update_all();
                        }
                    } else {
                        DEBUG_LOG("REPORT_VLAN_IPV6_ADDRESSES: unchanged, skipped\n");
                    }
                } else {
                    snprintf(response, sizeof(response), "ERROR=Unknown command");
                }
            }

            if(is_change) {
                if(config_update_from_global(page_data) > 0) {
                    data_manager_reset_poll_timer();
                }
            }
            // Send response
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