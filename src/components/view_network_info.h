#ifndef VIEW_NETWORK_INFO_H
#define VIEW_NETWORK_INFO_H

#include "../../lvgl/lvgl.h"
#include "../core/page_manager.h"
#include <stdbool.h>

typedef struct {
    char ipv4[20];
    char ipv6[45];
    char mac[50];
    bool has_ipv6;
} NetInfoPlainData;

typedef struct {
    char vlan_tag[32];
    char ipv4[20];
    char ipv6[45];
    char mac[50];
    bool has_ipv6;
} VlanPageData;

typedef struct {
    int vlan_count;
    int active_index;
    NetInfoPlainData plain;
    VlanPageData vlan_pages[2];
} NetworkInfoState;

void view_network_info_init(lv_obj_t * parent);
void view_network_info_apply(const NetworkInfoState * state);
bool network_info_is_online(const NetworkInfoState * state, const GlobalData * data);
bool view_network_info_handle_horizontal_swipe(int dx);
lv_obj_t * view_network_info_get_touch_target(void);
lv_obj_t * view_network_info_get_swipe_area(void);
int view_network_info_get_vlan_count(void);

#endif
