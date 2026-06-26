#include "view_network_info.h"
#include "../utils/font_util.h"
#include "../utils/ui_layout.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    lv_obj_t * parent;
    lv_obj_t * plain_ip_label;
    lv_obj_t * plain_ip_value;
    lv_obj_t * plain_ipv6_value;
    lv_obj_t * plain_mac_label;
    lv_obj_t * plain_mac_value;
    lv_obj_t * vlan_block;
    lv_obj_t * vlan_tag;
    lv_obj_t * vlan_ip_label;
    lv_obj_t * vlan_ip_value;
    lv_obj_t * vlan_ipv6_value;
    lv_obj_t * vlan_mac_label;
    lv_obj_t * vlan_mac_value;
    lv_obj_t * pager_row;
    lv_obj_t * pager_dot[2];
    lv_obj_t * swipe_area;
    lv_style_t style_label_small;
    NetworkInfoState cached;
    int swipe_active_index;
} NetworkInfoView;

static NetworkInfoView g_net;

static void enable_event_bubble(lv_obj_t * obj)
{
    if(obj) {
        lv_obj_add_flag(obj, LV_OBJ_FLAG_EVENT_BUBBLE);
    }
}

static bool page_has_address(const char * ipv4, const char * ipv6)
{
    return (ipv4 && ipv4[0] != '\0') || (ipv6 && ipv6[0] != '\0');
}

static bool plain_has_ipv6(const NetInfoPlainData * plain)
{
    return plain->ipv6[0] != '\0';
}

static bool vlan_page_has_ipv6(const VlanPageData * page)
{
    return page->has_ipv6 && page->ipv6[0] != '\0';
}

static int field_y(bool has_ipv6, int kind)
{
    switch(kind) {
        case 0: return has_ipv6 ? UI_PLAIN_Y_IP_HAS_V6 : UI_PLAIN_Y_IP_NO_V6;
        case 1: return has_ipv6 ? UI_PLAIN_Y_IP_VAL_HAS_V6 : UI_PLAIN_Y_IP_VAL_NO_V6;
        case 2: return has_ipv6 ? UI_PLAIN_Y_MAC_HAS_V6 : UI_PLAIN_Y_MAC_NO_V6;
        case 3: return has_ipv6 ? UI_PLAIN_Y_MAC_VAL_HAS_V6 : UI_PLAIN_Y_MAC_VAL_NO_V6;
        default: return 0;
    }
}

static void style_value_text(lv_obj_t * label, bool active)
{
    lv_obj_set_style_text_color(label,
        lv_color_hex(active ? UI_COLOR_TEXT : UI_COLOR_TEXT_INACTIVE), 0);
}

static void set_ipv4_label(lv_obj_t * label, const char * ip)
{
    if(ip && ip[0] != '\0') {
        lv_label_set_text(label, ip);
        style_value_text(label, true);
    } else {
        lv_label_set_text(label, "--.--.--.--");
        style_value_text(label, false);
    }
}

static void apply_plain_layout(void)
{
    const NetInfoPlainData * plain = &g_net.cached.plain;
    bool has_ipv6 = plain_has_ipv6(plain);

    lv_obj_add_flag(g_net.vlan_block, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(g_net.plain_ip_label, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(g_net.plain_ip_value, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(g_net.plain_mac_label, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(g_net.plain_mac_value, LV_OBJ_FLAG_HIDDEN);

    lv_obj_set_y(g_net.plain_ip_label, field_y(has_ipv6, 0));
    lv_obj_set_y(g_net.plain_ip_value, field_y(has_ipv6, 1));
    lv_obj_set_y(g_net.plain_mac_label, field_y(has_ipv6, 2));
    lv_obj_set_y(g_net.plain_mac_value, field_y(has_ipv6, 3));

    set_ipv4_label(g_net.plain_ip_value, plain->ipv4);
    lv_label_set_text(g_net.plain_ipv6_value, plain->ipv6);
    if(has_ipv6) {
        lv_obj_clear_flag(g_net.plain_ipv6_value, LV_OBJ_FLAG_HIDDEN);
        style_value_text(g_net.plain_ipv6_value, true);
    } else {
        lv_obj_add_flag(g_net.plain_ipv6_value, LV_OBJ_FLAG_HIDDEN);
    }

    lv_label_set_text(g_net.plain_mac_value, plain->mac);
    style_value_text(g_net.plain_mac_value, plain->mac[0] != '\0');
}

static void update_pager_dots(int active_index)
{
    if(g_net.cached.vlan_count != 2) {
        lv_obj_add_flag(g_net.pager_row, LV_OBJ_FLAG_HIDDEN);
        return;
    }

    lv_obj_clear_flag(g_net.pager_row, LV_OBJ_FLAG_HIDDEN);
    for(int i = 0; i < 2; i++) {
        lv_color_t color = (i == active_index)
            ? lv_color_hex(UI_COLOR_PAGER_ACTIVE)
            : lv_color_hex(UI_COLOR_PAGER_INACTIVE);
        lv_obj_set_style_bg_color(g_net.pager_dot[i], color, LV_PART_MAIN);
    }
}

static void refresh_vlan_tag(const char * text)
{
    const lv_font_t * font = font_sls(FS_FONT_MEDIUM_10);
    lv_coord_t line_h = lv_font_get_line_height(font);
    lv_coord_t vpad = (UI_VLAN_TAG_H - line_h) / 2;

    if(vpad < 0) {
        vpad = 0;
    }

    lv_label_set_text(g_net.vlan_tag, text ? text : "");
    lv_obj_set_style_text_align(g_net.vlan_tag, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);
    lv_obj_set_style_pad_left(g_net.vlan_tag, UI_VLAN_TAG_PAD_X, LV_PART_MAIN);
    lv_obj_set_style_pad_right(g_net.vlan_tag, UI_VLAN_TAG_PAD_X, LV_PART_MAIN);
    lv_obj_set_style_pad_top(g_net.vlan_tag, vpad, LV_PART_MAIN);
    lv_obj_set_style_pad_bottom(g_net.vlan_tag, UI_VLAN_TAG_H - line_h - vpad, LV_PART_MAIN);
    lv_obj_set_size(g_net.vlan_tag, LV_SIZE_CONTENT, UI_VLAN_TAG_H);
    lv_obj_set_pos(g_net.vlan_tag, UI_VLAN_TAG_X, UI_VLAN_TAG_Y);
    lv_obj_update_layout(g_net.vlan_tag);
}

static int vlan_field_y(bool has_ipv6, int kind)
{
    if(!has_ipv6) {
        return field_y(false, kind);
    }

    switch(kind) {
        case 0: return UI_VLAN_Y_IP_HAS_V6;
        case 1: return UI_VLAN_Y_IP_VAL_HAS_V6;
        case 2: return UI_VLAN_Y_MAC_HAS_V6;
        case 3: return UI_VLAN_Y_MAC_VAL_HAS_V6;
        default: return 0;
    }
}

static void layout_vlan_fields(bool has_ipv6)
{
    lv_obj_set_pos(g_net.vlan_ip_label, UI_PLAIN_X, vlan_field_y(has_ipv6, 0));
    lv_obj_set_pos(g_net.vlan_ip_value, UI_PLAIN_X, vlan_field_y(has_ipv6, 1));
    lv_obj_set_pos(g_net.vlan_mac_label, UI_PLAIN_X, vlan_field_y(has_ipv6, 2));
    lv_obj_set_pos(g_net.vlan_mac_value, UI_PLAIN_X, vlan_field_y(has_ipv6, 3));

    if(has_ipv6) {
        lv_obj_set_pos(g_net.vlan_ipv6_value, UI_PLAIN_X, UI_VLAN_Y_IPV6_HAS_V6);
        lv_obj_clear_flag(g_net.vlan_ipv6_value, LV_OBJ_FLAG_HIDDEN);
    } else {
        lv_obj_add_flag(g_net.vlan_ipv6_value, LV_OBJ_FLAG_HIDDEN);
    }
}

static void apply_vlan_layout(void)
{
    int idx = g_net.swipe_active_index;
    if(idx < 0 || idx >= g_net.cached.vlan_count) {
        idx = 0;
    }

    const VlanPageData * page = &g_net.cached.vlan_pages[idx];
    bool has_ipv6 = vlan_page_has_ipv6(page);

    lv_obj_add_flag(g_net.plain_ip_label, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(g_net.plain_ip_value, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(g_net.plain_ipv6_value, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(g_net.plain_mac_label, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(g_net.plain_mac_value, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(g_net.vlan_block, LV_OBJ_FLAG_HIDDEN);

    refresh_vlan_tag(page->vlan_tag);
    set_ipv4_label(g_net.vlan_ip_value, page->ipv4);

    if(has_ipv6) {
        lv_label_set_text(g_net.vlan_ipv6_value, page->ipv6);
        style_value_text(g_net.vlan_ipv6_value, true);
    }

    lv_label_set_text(g_net.vlan_mac_value, page->mac);
    style_value_text(g_net.vlan_mac_value, page->mac[0] != '\0');

    layout_vlan_fields(has_ipv6);
    update_pager_dots(idx);
}

static void apply_internal(void)
{
    if(g_net.cached.vlan_count > 0) {
        apply_vlan_layout();
    } else {
        apply_plain_layout();
    }
}

static void init_field_label(lv_obj_t * label, const char * text, int y)
{
    lv_label_set_text(label, text);
    lv_obj_set_style_text_font(label, font_sls(FS_FONT_REGULAR_10), 0);
    lv_obj_add_style(label, &g_net.style_label_small, LV_PART_MAIN);
    lv_obj_set_pos(label, UI_PLAIN_X, y);
    enable_event_bubble(label);
}

static void init_value_label(lv_obj_t * label, FsFontType font, int y)
{
    lv_obj_set_style_text_font(label, font_sls(font), 0);
    lv_obj_set_style_text_color(label, lv_color_hex(UI_COLOR_TEXT), 0);
    lv_obj_set_pos(label, UI_PLAIN_X, y);
    enable_event_bubble(label);
}

void view_network_info_init(lv_obj_t * parent)
{
    memset(&g_net, 0, sizeof(g_net));
    g_net.parent = parent;

    lv_style_init(&g_net.style_label_small);
    lv_style_set_text_font(&g_net.style_label_small, &lv_font_montserrat_10);
    lv_style_set_text_color(&g_net.style_label_small, lv_color_white());

    g_net.plain_ip_label = lv_label_create(parent);
    init_field_label(g_net.plain_ip_label, "Device IP", UI_PLAIN_Y_IP_NO_V6);

    g_net.plain_ip_value = lv_label_create(parent);
    init_value_label(g_net.plain_ip_value, FS_FONT_MEDIUM_28, UI_PLAIN_Y_IP_VAL_NO_V6);

    g_net.plain_ipv6_value = lv_label_create(parent);
    lv_obj_set_style_text_font(g_net.plain_ipv6_value, font_sls(FS_FONT_MEDIUM_12), 0);
    lv_obj_set_style_text_color(g_net.plain_ipv6_value, lv_color_hex(UI_COLOR_TEXT), 0);
    lv_obj_set_pos(g_net.plain_ipv6_value, UI_PLAIN_X, UI_PLAIN_IPV6_Y);
    lv_obj_set_size(g_net.plain_ipv6_value, UI_PLAIN_IPV6_W, UI_PLAIN_IPV6_H);
    enable_event_bubble(g_net.plain_ipv6_value);

    g_net.plain_mac_label = lv_label_create(parent);
    init_field_label(g_net.plain_mac_label, "Device MAC", UI_PLAIN_Y_MAC_NO_V6);

    g_net.plain_mac_value = lv_label_create(parent);
    init_value_label(g_net.plain_mac_value, FS_FONT_MEDIUM_22, UI_PLAIN_Y_MAC_VAL_NO_V6);

    /* Transparent grouping root; children use same absolute coords as plain mode */
    g_net.vlan_block = lv_obj_create(parent);
    lv_obj_set_pos(g_net.vlan_block, 0, 0);
    lv_obj_set_size(g_net.vlan_block, LV_PCT(100), LV_PCT(100));
    lv_obj_set_style_bg_opa(g_net.vlan_block, LV_OPA_TRANSP, LV_PART_MAIN);
    lv_obj_set_style_border_width(g_net.vlan_block, 0, LV_PART_MAIN);
    lv_obj_set_style_pad_all(g_net.vlan_block, 0, LV_PART_MAIN);
    lv_obj_clear_flag(g_net.vlan_block, LV_OBJ_FLAG_SCROLLABLE);
    enable_event_bubble(g_net.vlan_block);

    g_net.vlan_tag = lv_label_create(g_net.vlan_block);
    lv_obj_set_style_text_font(g_net.vlan_tag, font_sls(FS_FONT_MEDIUM_10), 0);
    lv_obj_set_style_text_color(g_net.vlan_tag, lv_color_hex(UI_COLOR_VLAN_ACCENT), 0);
    lv_obj_set_style_border_width(g_net.vlan_tag, 1, LV_PART_MAIN);
    lv_obj_set_style_border_color(g_net.vlan_tag, lv_color_hex(UI_COLOR_VLAN_ACCENT), LV_PART_MAIN);
    lv_obj_set_style_radius(g_net.vlan_tag, UI_VLAN_TAG_RADIUS, LV_PART_MAIN);
    enable_event_bubble(g_net.vlan_tag);
    refresh_vlan_tag("Primary VLAN 40");

    g_net.vlan_ip_label = lv_label_create(g_net.vlan_block);
    lv_label_set_text(g_net.vlan_ip_label, "Device IP");
    lv_obj_set_style_text_font(g_net.vlan_ip_label, font_sls(FS_FONT_REGULAR_10), 0);
    lv_obj_add_style(g_net.vlan_ip_label, &g_net.style_label_small, LV_PART_MAIN);
    enable_event_bubble(g_net.vlan_ip_label);

    g_net.vlan_ip_value = lv_label_create(g_net.vlan_block);
    lv_obj_set_style_text_font(g_net.vlan_ip_value, font_sls(FS_FONT_MEDIUM_28), 0);
    enable_event_bubble(g_net.vlan_ip_value);

    g_net.vlan_ipv6_value = lv_label_create(g_net.vlan_block);
    lv_obj_set_style_text_font(g_net.vlan_ipv6_value, font_sls(FS_FONT_MEDIUM_12), 0);
    lv_obj_set_style_text_color(g_net.vlan_ipv6_value, lv_color_hex(UI_COLOR_TEXT), 0);
    lv_obj_set_size(g_net.vlan_ipv6_value, UI_PLAIN_IPV6_W, UI_PLAIN_IPV6_H);
    enable_event_bubble(g_net.vlan_ipv6_value);

    g_net.vlan_mac_label = lv_label_create(g_net.vlan_block);
    lv_label_set_text(g_net.vlan_mac_label, "Device MAC");
    lv_obj_set_style_text_font(g_net.vlan_mac_label, font_sls(FS_FONT_REGULAR_10), 0);
    lv_obj_add_style(g_net.vlan_mac_label, &g_net.style_label_small, LV_PART_MAIN);
    enable_event_bubble(g_net.vlan_mac_label);

    g_net.vlan_mac_value = lv_label_create(g_net.vlan_block);
    lv_obj_set_style_text_font(g_net.vlan_mac_value, font_sls(FS_FONT_MEDIUM_22), 0);
    lv_obj_set_style_text_color(g_net.vlan_mac_value, lv_color_hex(UI_COLOR_TEXT), 0);
    enable_event_bubble(g_net.vlan_mac_value);

    g_net.pager_row = lv_obj_create(g_net.vlan_block);
    lv_obj_set_size(g_net.pager_row, UI_PLAIN_IPV6_W, UI_VLAN_DOT_SIZE);
    lv_obj_set_pos(g_net.pager_row, UI_PLAIN_X, UI_VLAN_PAGER_Y);
    lv_obj_set_style_bg_opa(g_net.pager_row, LV_OPA_TRANSP, LV_PART_MAIN);
    lv_obj_set_style_border_width(g_net.pager_row, 0, LV_PART_MAIN);
    lv_obj_set_style_pad_all(g_net.pager_row, 0, LV_PART_MAIN);
    lv_obj_clear_flag(g_net.pager_row, LV_OBJ_FLAG_SCROLLABLE);
    enable_event_bubble(g_net.pager_row);

    for(int i = 0; i < 2; i++) {
        g_net.pager_dot[i] = lv_obj_create(g_net.pager_row);
        lv_obj_set_size(g_net.pager_dot[i], UI_VLAN_DOT_SIZE, UI_VLAN_DOT_SIZE);
        lv_obj_set_style_radius(g_net.pager_dot[i], LV_RADIUS_CIRCLE, LV_PART_MAIN);
        lv_obj_set_style_border_width(g_net.pager_dot[i], 0, LV_PART_MAIN);
        lv_obj_set_pos(g_net.pager_dot[i],
            (UI_PLAIN_IPV6_W - (UI_VLAN_DOT_SIZE * 2 + UI_VLAN_DOT_GAP)) / 2
            + i * (UI_VLAN_DOT_SIZE + UI_VLAN_DOT_GAP), 0);
        enable_event_bubble(g_net.pager_dot[i]);
    }

    g_net.swipe_area = lv_obj_create(parent);
    lv_obj_set_pos(g_net.swipe_area, 0, UI_VLAN_SWIPE_Y);
    lv_obj_set_size(g_net.swipe_area, LV_PCT(100), UI_VLAN_SWIPE_H);
    lv_obj_set_style_bg_opa(g_net.swipe_area, LV_OPA_TRANSP, LV_PART_MAIN);
    lv_obj_set_style_border_width(g_net.swipe_area, 0, LV_PART_MAIN);
    lv_obj_clear_flag(g_net.swipe_area, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_add_flag(g_net.swipe_area, LV_OBJ_FLAG_CLICKABLE);
    enable_event_bubble(g_net.swipe_area);

    lv_obj_add_flag(g_net.vlan_block, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(g_net.pager_row, LV_OBJ_FLAG_HIDDEN);
}

void view_network_info_apply(const NetworkInfoState * state)
{
    if(!state) {
        return;
    }

    int prev_count = g_net.cached.vlan_count;
    g_net.cached = *state;

    if(g_net.cached.vlan_count != 2) {
        g_net.swipe_active_index = 0;
    } else if(prev_count != 2) {
        g_net.swipe_active_index = g_net.cached.active_index;
        if(g_net.swipe_active_index < 0 || g_net.swipe_active_index > 1) {
            g_net.swipe_active_index = 0;
        }
    }

    apply_internal();
}

bool network_info_is_online(const NetworkInfoState * state, const GlobalData * data)
{
    if(!state || !data) {
        return false;
    }

    if(state->vlan_count == 0) {
        return page_has_address(data->ip, data->ipv6);
    }

    for(int i = 0; i < state->vlan_count; i++) {
        if(page_has_address(state->vlan_pages[i].ipv4, state->vlan_pages[i].ipv6)) {
            return true;
        }
    }
    return false;
}

bool view_network_info_handle_horizontal_swipe(int dx)
{
    if(g_net.cached.vlan_count != 2) {
        return false;
    }

    if(dx <= -UI_SWIPE_H_THRESHOLD) {
        g_net.swipe_active_index = (g_net.swipe_active_index + 1) % 2;
    } else if(dx >= UI_SWIPE_H_THRESHOLD) {
        g_net.swipe_active_index = (g_net.swipe_active_index + 2 - 1) % 2;
    } else {
        return false;
    }

    apply_vlan_layout();
    return true;
}

lv_obj_t * view_network_info_get_touch_target(void)
{
    return g_net.parent;
}

lv_obj_t * view_network_info_get_swipe_area(void)
{
    return g_net.swipe_area;
}

int view_network_info_get_vlan_count(void)
{
    return g_net.cached.vlan_count;
}
