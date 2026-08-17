#include "data_manager.h"
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "../components/view_hold_action.h"
#include "../utils/command.h"
#include "network_vlan.h"

static time_t g_last_update = 0;
static DataUpdateCallback g_update_callback = NULL;
static const int UPDATE_INTERVAL = 2;

static void update_ip_address(GlobalData * data)
{
    char carrier[8] = {0};
    char new_ip[20] = {0};

    /* Link down: clear stale IP immediately (#1). */
    if(run_command("cat /sys/class/net/eth0/carrier 2>/dev/null", carrier, sizeof(carrier)) == 0
       && carrier[0] == '0') {
        if(data->ip[0] != '\0') {
            memset(data->ip, 0, sizeof(data->ip));
        }
        return;
    }

    if(run_command("ifconfig eth0 | grep 'inet addr:' | awk '{print $2}' | cut -d':' -f2",
                   new_ip, sizeof(new_ip)) == 0) {
        if(strcmp(data->ip, new_ip) != 0) {
            strncpy(data->ip, new_ip, sizeof(data->ip) - 1);
        }
    } else if(data->ip[0] != '\0') {
        memset(data->ip, 0, sizeof(data->ip));
    }
}

static void update_ipv6_address(GlobalData * data)
{
    char * new_ip6 = get_ipv6_address();

    if(new_ip6 == NULL) {
        if(data->ipv6[0] != '\0') {
            data->ipv6[0] = '\0';
        }
    } else if(strcmp(data->ipv6, new_ip6) != 0) {
        strncpy(data->ipv6, new_ip6, sizeof(data->ipv6) - 1);
        data->ipv6[sizeof(data->ipv6) - 1] = '\0';
    }
}

static void update_mac_address(GlobalData * data)
{
    char new_mac[50] = {0};
    if(run_command("ifconfig eth0 | grep 'HWaddr' | awk '{print $5}'",
                   new_mac, sizeof(new_mac)) == 0) {
        if(strcmp(data->mac, new_mac) != 0) {
            strncpy(data->mac, new_mac, sizeof(data->mac) - 1);
        }
    }
}

static void update_usb_status(GlobalData * data)
{
    char status[10] = {0};
    bool new_status = false;

    if(run_command("grep -o 'USB=[01]' /sys/class/extcon/extcon0/state | cut -d= -f2",
                  status, sizeof(status)) == 0) {
        new_status = (status[0] == '1');
    }

    if(data->is_connect_usb != new_status) {
        data->is_connect_usb = new_status;
    }
}

static void update_app_info(GlobalData * data)
{
    char app_version[100] = {0};
    char system_version[100] = {0};
    char serial_number[256] = {0};

    run_command("cat /etc/version/app", app_version, sizeof(app_version));
    run_command("cat /etc/version/system", system_version, sizeof(system_version));
    run_command("grep -i 'serial' /proc/cpuinfo | head -1 | awk -F':' '{print $2}' | sed 's/^[ \\t]*//;s/[ \\t]*$//'",
                serial_number, sizeof(serial_number));

    if(strcmp(data->app_version, app_version) != 0) {
        strncpy(data->app_version, app_version, sizeof(data->app_version) - 1);
    }
    if(strcmp(data->os_version, system_version) != 0) {
        strncpy(data->os_version, system_version, sizeof(data->os_version) - 1);
    }
    if(strcmp(data->serial_number, serial_number) != 0) {
        strncpy(data->serial_number, serial_number, sizeof(data->serial_number) - 1);
    }
}

static void update_hdmi_status(GlobalData * data)
{
    char status[20] = {0};
    bool new_status = false;

    if(run_command("grep -o \"status: [a-z]\\+\" /sys/kernel/debug/rk628/3-0050/hdmirx/status | cut -d\" \" -f2",
                  status, sizeof(status)) == 0) {
        new_status = (strcmp(status, "plugin") == 0);
    }

    if(data->is_connect_hdmi != new_status) {
        data->is_connect_hdmi = new_status;
    }
}

void data_manager_init(void)
{
    g_last_update = 0;
}

void data_manager_reset_poll_timer(void)
{
    g_last_update = 0;
}

void data_manager_register_callback(DataUpdateCallback cb)
{
    g_update_callback = cb;
}

void data_manager_update(void)
{
    static time_t last_run = 0;
    time_t now = time(NULL);

    /* Defense in depth: never popen/sysfs poll during hold/busy on LVGL thread. */
    if(view_hold_action_blocks_background()) {
        return;
    }

    if(now - last_run < UPDATE_INTERVAL) {
        return;
    }
    last_run = now;

    GlobalData * data = page_get_global_data();
    GlobalData old_data = *data;

    update_ip_address(data);
    update_ipv6_address(data);
    update_mac_address(data);
    if(data->vlan_enabled && data->primary_vlan_id > 0) {
        network_vlan_refresh_addresses(data);
    }
    update_usb_status(data);
    update_hdmi_status(data);
    update_app_info(data);

    bool new_net_status = (data->ip[0] != '\0') || (data->ipv6[0] != '\0');
    if(data->is_connect_net != new_net_status) {
        data->is_connect_net = new_net_status;
    }

    if(memcmp(data, &old_data, sizeof(GlobalData)) != 0) {
        g_last_update = now;
        if(g_update_callback) {
            g_update_callback();
        }
    }
}

bool data_manager_get_network_status(void)
{
    GlobalData * data = page_get_global_data();
    return data->is_connect_net;
}

time_t data_manager_get_last_update_time(void)
{
    return g_last_update;
}
