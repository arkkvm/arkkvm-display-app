#include "network_vlan.h"
#include "../utils/command.h"
#include "../utils/cJSON.h"
#include "../utils/net_util.h"
#include "../utils/debug.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int vlan_id;
    char ipv6[45];
    bool has_ipv6;
} VlanIpv6CacheEntry;

static VlanIpv6CacheEntry g_vlan_ipv6_cache[2];
static int g_vlan_ipv6_cache_count = 0;

static void clear_vlan_ipv6_cache(void)
{
    memset(g_vlan_ipv6_cache, 0, sizeof(g_vlan_ipv6_cache));
    g_vlan_ipv6_cache_count = 0;
}

static void set_vlan_ipv6_cache_entry(int vlan_id, const char * ipv6)
{
    int i;

    for(i = 0; i < g_vlan_ipv6_cache_count; i++) {
        if(g_vlan_ipv6_cache[i].vlan_id == vlan_id) {
            g_vlan_ipv6_cache[i].ipv6[0] = '\0';
            g_vlan_ipv6_cache[i].has_ipv6 = false;
            if(ipv6 && ipv6[0] != '\0') {
                strncpy(g_vlan_ipv6_cache[i].ipv6, ipv6, sizeof(g_vlan_ipv6_cache[i].ipv6) - 1);
                g_vlan_ipv6_cache[i].has_ipv6 = true;
            }
            return;
        }
    }

    if(g_vlan_ipv6_cache_count >= 2) {
        return;
    }

    g_vlan_ipv6_cache[g_vlan_ipv6_cache_count].vlan_id = vlan_id;
    if(ipv6 && ipv6[0] != '\0') {
        strncpy(g_vlan_ipv6_cache[g_vlan_ipv6_cache_count].ipv6, ipv6,
                sizeof(g_vlan_ipv6_cache[g_vlan_ipv6_cache_count].ipv6) - 1);
        g_vlan_ipv6_cache[g_vlan_ipv6_cache_count].has_ipv6 = true;
    }
    g_vlan_ipv6_cache_count++;
}

static void apply_vlan_ipv6_to_page(GlobalVlanPage * page, int vlan_id)
{
    int i;

    if(!page) {
        return;
    }

    page->ipv6[0] = '\0';
    page->has_ipv6 = false;

    for(i = 0; i < g_vlan_ipv6_cache_count; i++) {
        if(g_vlan_ipv6_cache[i].vlan_id == vlan_id && g_vlan_ipv6_cache[i].has_ipv6) {
            strncpy(page->ipv6, g_vlan_ipv6_cache[i].ipv6, sizeof(page->ipv6) - 1);
            page->has_ipv6 = true;
            return;
        }
    }
}

static void apply_ipv6_cache_to_pages(GlobalData * data)
{
    if(!data || data->vlan_count <= 0) {
        return;
    }

    apply_vlan_ipv6_to_page(&data->vlan_pages[0], data->primary_vlan_id);
    if(data->vlan_count >= 2) {
        apply_vlan_ipv6_to_page(&data->vlan_pages[1], data->secondary_vlan_id);
    }
}

static void clear_vlan_page_ipv6(GlobalData * data)
{
    int i;

    if(!data) {
        return;
    }

    for(i = 0; i < data->vlan_count && i < 2; i++) {
        data->vlan_pages[i].ipv6[0] = '\0';
        data->vlan_pages[i].has_ipv6 = false;
    }
}

static const char * default_mac(const GlobalData * data)
{
    if(data && data->mac[0] != '\0') {
        return data->mac;
    }
    return "30:BC:4F:7A:39:44";
}

static void set_vlan_page(GlobalVlanPage * page, const char * tag, const char * ipv4,
                          const char * ipv6, const char * mac)
{
    memset(page, 0, sizeof(*page));
    strncpy(page->vlan_tag, tag, sizeof(page->vlan_tag) - 1);
    strncpy(page->ipv4, ipv4, sizeof(page->ipv4) - 1);
    strncpy(page->mac, mac, sizeof(page->mac) - 1);
    if(ipv6 && ipv6[0] != '\0') {
        strncpy(page->ipv6, ipv6, sizeof(page->ipv6) - 1);
        page->has_ipv6 = true;
    }
}

static bool read_interface_ipv4(const char * iface, char * out, size_t out_size)
{
    char cmd[128];

    if(!iface || !out || out_size == 0) {
        return false;
    }

    snprintf(cmd, sizeof(cmd),
             "ifconfig %s | grep 'inet addr:' | awk '{print $2}' | cut -d':' -f2",
             iface);
    out[0] = '\0';
    return run_command(cmd, out, out_size) == 0 && out[0] != '\0';
}

static bool read_interface_mac(const char * iface, char * out, size_t out_size)
{
    char cmd[128];

    if(!iface || !out || out_size == 0) {
        return false;
    }

    snprintf(cmd, sizeof(cmd),
             "ifconfig %s | grep 'HWaddr' | awk '{print $5}'",
             iface);
    out[0] = '\0';
    return run_command(cmd, out, out_size) == 0 && out[0] != '\0';
}

static void read_vlan_page_from_iface(GlobalVlanPage * page, const char * iface,
                                      const char * tag, const char * mac_fallback)
{
    char ipv4[20] = {0};
    char mac[50] = {0};

    read_interface_ipv4(iface, ipv4, sizeof(ipv4));
    if(!read_interface_mac(iface, mac, sizeof(mac))) {
        strncpy(mac, mac_fallback, sizeof(mac) - 1);
    }

    set_vlan_page(page, tag, ipv4, NULL, mac);
}

void network_vlan_clear(GlobalData * data)
{
    if(!data) {
        return;
    }
    data->vlan_enabled = false;
    data->primary_vlan_id = 0;
    data->secondary_vlan_id = 0;
    data->vlan_count = 0;
    data->vlan_ipc_active = true;
    memset(data->vlan_pages, 0, sizeof(data->vlan_pages));
    clear_vlan_ipv6_cache();
}

bool network_vlan_apply_report(GlobalData * data, const char * json)
{
    cJSON * root;
    cJSON * enabled_item;
    bool enabled;
    int primary = 0;
    int secondary = 0;

    if(!data || !json || json[0] == '\0') {
        return false;
    }

    root = cJSON_Parse(json);
    if(!root) {
        return false;
    }

    enabled_item = cJSON_GetObjectItem(root, "vlan_enabled");
    if(!enabled_item || !cJSON_IsBool(enabled_item)) {
        cJSON_Delete(root);
        return false;
    }

    enabled = cJSON_IsTrue(enabled_item);
    if(enabled) {
        cJSON * primary_item = cJSON_GetObjectItem(root, "primaryVlanId");
        cJSON * secondary_item = cJSON_GetObjectItem(root, "secondaryVlanId");

        if(primary_item && cJSON_IsNumber(primary_item)) {
            primary = (int)primary_item->valuedouble;
        }
        if(secondary_item && cJSON_IsNumber(secondary_item)) {
            secondary = (int)secondary_item->valuedouble;
        }
    }

    cJSON_Delete(root);

    data->vlan_enabled = enabled;
    data->primary_vlan_id = enabled ? primary : 0;
    data->secondary_vlan_id = (enabled && secondary > 0) ? secondary : 0;
    data->vlan_ipc_active = true;

    if(!enabled || primary <= 0) {
        network_vlan_clear(data);
        data->vlan_ipc_active = true;
        return true;
    }

    network_vlan_refresh_addresses(data);
    return true;
}

bool network_vlan_apply_ipv6_report(GlobalData * data, const char * json)
{
    cJSON * root;
    int size;
    int i;

    if(!data || !json) {
        return false;
    }

    root = cJSON_Parse(json);
    if(!root || !cJSON_IsArray(root)) {
        DEBUG_LOG("REPORT_VLAN_IPV6_ADDRESSES: invalid JSON\n");
        if(root) {
            cJSON_Delete(root);
        }
        return false;
    }

    clear_vlan_ipv6_cache();
    size = cJSON_GetArraySize(root);
    DEBUG_LOG("REPORT_VLAN_IPV6_ADDRESSES: received %d vlan entr%s\n",
           size, size == 1 ? "y" : "ies");
    for(i = 0; i < size; i++) {
        cJSON * item = cJSON_GetArrayItem(root, i);
        cJSON * vlan_id_item;
        cJSON * ipv6_item;
        char * ipv6_json;
        char * show_ipv6;
        int vlan_id;

        if(!item || !cJSON_IsObject(item)) {
            continue;
        }

        vlan_id_item = cJSON_GetObjectItem(item, "vlanId");
        ipv6_item = cJSON_GetObjectItem(item, "ipv6");
        if(!vlan_id_item || !cJSON_IsNumber(vlan_id_item) || !ipv6_item || !cJSON_IsArray(ipv6_item)) {
            continue;
        }

        vlan_id = (int)vlan_id_item->valuedouble;
        ipv6_json = cJSON_PrintUnformatted(ipv6_item);
        if(!ipv6_json) {
            set_vlan_ipv6_cache_entry(vlan_id, NULL);
            DEBUG_LOG("REPORT_VLAN_IPV6_ADDRESSES: vlanId=%d show=(none)\n", vlan_id);
            continue;
        }

        show_ipv6 = handle_ipv6_report(ipv6_json);
        free(ipv6_json);
        set_vlan_ipv6_cache_entry(vlan_id, show_ipv6);
        DEBUG_LOG("REPORT_VLAN_IPV6_ADDRESSES: vlanId=%d show=%s\n",
               vlan_id, show_ipv6 ? show_ipv6 : "(none)");
        if(show_ipv6) {
            free(show_ipv6);
        }
    }

    cJSON_Delete(root);

    if(size == 0) {
        DEBUG_LOG("REPORT_VLAN_IPV6_ADDRESSES: cleared all vlan IPv6\n");
        clear_vlan_page_ipv6(data);
    } else {
        apply_ipv6_cache_to_pages(data);
    }

    return true;
}

void network_vlan_refresh_addresses(GlobalData * data)
{
    char iface[16];
    char tag[32];
    int count;
    const char * mac_fallback;

    if(!data || !data->vlan_enabled || data->primary_vlan_id <= 0) {
        return;
    }

    mac_fallback = default_mac(data);
    count = 1;
    if(data->secondary_vlan_id > 0 && data->secondary_vlan_id != data->primary_vlan_id) {
        count = 2;
    }

    data->vlan_count = count;
    memset(data->vlan_pages, 0, sizeof(data->vlan_pages));

    snprintf(iface, sizeof(iface), "eth0.%d", data->primary_vlan_id);
    snprintf(tag, sizeof(tag), "Primary VLAN %d", data->primary_vlan_id);
    read_vlan_page_from_iface(&data->vlan_pages[0], iface, tag, mac_fallback);

    if(count >= 2) {
        snprintf(iface, sizeof(iface), "eth0.%d", data->secondary_vlan_id);
        snprintf(tag, sizeof(tag), "Secondary VLAN %d", data->secondary_vlan_id);
        read_vlan_page_from_iface(&data->vlan_pages[1], iface, tag, mac_fallback);
    }

    apply_ipv6_cache_to_pages(data);
}

static void copy_vlan_page(VlanPageData * dst, const GlobalVlanPage * src)
{
    strncpy(dst->vlan_tag, src->vlan_tag, sizeof(dst->vlan_tag) - 1);
    strncpy(dst->ipv4, src->ipv4, sizeof(dst->ipv4) - 1);
    strncpy(dst->ipv6, src->ipv6, sizeof(dst->ipv6) - 1);
    strncpy(dst->mac, src->mac, sizeof(dst->mac) - 1);
    dst->has_ipv6 = src->has_ipv6;
}

void network_vlan_fill_state(const GlobalData * data, NetworkInfoState * state)
{
    if(!data || !state || data->vlan_count <= 0) {
        return;
    }

    state->vlan_count = data->vlan_count;
    if(state->vlan_count > 2) {
        state->vlan_count = 2;
    }

    for(int i = 0; i < state->vlan_count; i++) {
        copy_vlan_page(&state->vlan_pages[i], &data->vlan_pages[i]);
    }
}
