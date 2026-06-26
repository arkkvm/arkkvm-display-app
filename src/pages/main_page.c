#include "main_page.h"
#include "../../lvgl/lvgl.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../components/view_net_dis.h"
#include "../components/view_network_info.h"
#include "../core/network_vlan.h"
#include "../utils/font_util.h"
#include "../utils/ui_layout.h"
#include "../utils/ui_preview.h"

static MainPageUI ui;
static lv_style_t style_label_small;

LV_IMG_DECLARE(icon_logo);
LV_IMG_DECLARE(icon_hdmi);
LV_IMG_DECLARE(icon_hdmi_dis);
LV_IMG_DECLARE(icon_usb);
LV_IMG_DECLARE(icon_usb_dis);

static void fill_mock_vlan_page(VlanPageData * page, const char * tag, const char * mac)
{
    strncpy(page->vlan_tag, tag, sizeof(page->vlan_tag) - 1);
    strncpy(page->ipv4, "192.168.66.66", sizeof(page->ipv4) - 1);
    strncpy(page->ipv6, "2408:8402:3021:2701:7ad2:f3ff:fe1a:55c4", sizeof(page->ipv6) - 1);
    strncpy(page->mac, mac, sizeof(page->mac) - 1);
    page->has_ipv6 = true;
}

static void build_network_state(NetworkInfoState * state, GlobalData * data)
{
    memset(state, 0, sizeof(*state));

    if(data->vlan_count > 0) {
        network_vlan_fill_state(data, state);
        return;
    }

    if(data->vlan_ipc_active) {
        state->vlan_count = 0;
        strncpy(state->plain.ipv4, data->ip, sizeof(state->plain.ipv4) - 1);
        strncpy(state->plain.ipv6, data->ipv6, sizeof(state->plain.ipv6) - 1);
        strncpy(state->plain.mac, data->mac, sizeof(state->plain.mac) - 1);
        state->plain.has_ipv6 = (data->ipv6[0] != '\0');
        return;
    }

#if UI_VLAN_PREVIEW
    state->vlan_count = UI_VLAN_PREVIEW_COUNT;
    if(state->vlan_count < 1) {
        state->vlan_count = 1;
    }
    if(state->vlan_count > 2) {
        state->vlan_count = 2;
    }

    const char * mac = data->mac[0] ? data->mac : "30:BC:4F:7A:39:44";
    fill_mock_vlan_page(&state->vlan_pages[0], "Primary VLAN 40", mac);
    if(state->vlan_count >= 2) {
        fill_mock_vlan_page(&state->vlan_pages[1], "Secondary VLAN 4094", mac);
    }
#else
    state->vlan_count = 0;
    strncpy(state->plain.ipv4, data->ip, sizeof(state->plain.ipv4) - 1);
    strncpy(state->plain.ipv6, data->ipv6, sizeof(state->plain.ipv6) - 1);
    strncpy(state->plain.mac, data->mac, sizeof(state->plain.mac) - 1);
    state->plain.has_ipv6 = (data->ipv6[0] != '\0');
#endif
}

static void main_page_gesture_handler(lv_event_t * e);

static void register_page_gesture(lv_obj_t * obj)
{
    if(obj) {
        lv_obj_add_event_cb(obj, main_page_gesture_handler, LV_EVENT_ALL, NULL);
    }
}

static void main_page_gesture_handler(lv_event_t * e)
{
    lv_indev_t * indev = lv_event_get_indev(e);
    lv_indev_data_t data;
    lv_indev_get_point(indev, &data);

    static lv_coord_t start_x = 0;
    static lv_coord_t start_y = 0;
    static bool is_tracking = false;

    switch(lv_event_get_code(e)) {
        case LV_EVENT_LONG_PRESSED:
            PAGE_SWITCH(PAGE_SETTINGS);
            break;

        case LV_EVENT_PRESSED:
            start_x = data.point.x;
            start_y = data.point.y;
            is_tracking = true;
            break;

        case LV_EVENT_RELEASED:
            if(!is_tracking) {
                break;
            }
            is_tracking = false;

            lv_coord_t dx = data.point.x - start_x;
            lv_coord_t dy = data.point.y - start_y;
            lv_coord_t abs_dx = LV_ABS(dx);
            lv_coord_t abs_dy = LV_ABS(dy);

            if(abs_dx > abs_dy && abs_dx > UI_SWIPE_H_THRESHOLD) {
                if(view_network_info_get_vlan_count() == 2) {
                    view_network_info_handle_horizontal_swipe(dx);
                }
                break;
            }

            if(start_y < UI_SWIPE_TOP_ZONE_Y
                && dy > UI_SWIPE_DOWN_THRESHOLD
                && abs_dy > abs_dx) {
                PAGE_SWITCH(PAGE_SETTINGS);
            }
            break;

        default:
            break;
    }
}

void main_page_init(lv_obj_t* container)
{
    ui.container = container;

    GlobalData* data = page_get_global_data();

    lv_style_init(&style_label_small);
    lv_style_set_text_font(&style_label_small, &lv_font_montserrat_10);
    lv_style_set_text_color(&style_label_small, lv_color_white());

    ui.logo_img = lv_img_create(ui.container);
    lv_img_set_src(ui.logo_img, &icon_logo);
    lv_obj_set_pos(ui.logo_img, 26, 34);
    lv_obj_set_size(ui.logo_img, 100, 20);

    view_network_info_init(ui.container);

    ui.usb_label = lv_label_create(ui.container);
    lv_label_set_text(ui.usb_label, "USB");
    lv_obj_set_style_text_font(ui.usb_label, font_sls(FS_FONT_MEDIUM_10), 0);
    lv_obj_add_style(ui.usb_label, &style_label_small, LV_PART_MAIN);
    lv_obj_set_pos(ui.usb_label, 175 + 8, 27);
    lv_obj_set_size(ui.usb_label, 36, 10);
    lv_obj_set_style_text_align(ui.usb_label, LV_TEXT_ALIGN_CENTER, 0);

    ui.usb_img = lv_img_create(ui.container);
    lv_img_set_src(ui.usb_img, data->is_connect_usb ? &icon_usb : &icon_usb_dis);
    lv_obj_set_pos(ui.usb_img, 180, 42);
    lv_obj_set_size(ui.usb_img, 36, 30);

    ui.hdmi_label = lv_label_create(ui.container);
    lv_label_set_text(ui.hdmi_label, "HDMI");
    lv_obj_set_style_text_font(ui.hdmi_label, font_sls(FS_FONT_MEDIUM_10), 0);
    lv_obj_add_style(ui.hdmi_label, &style_label_small, LV_PART_MAIN);
    lv_obj_set_pos(ui.hdmi_label, 221 + 8, 27);
    lv_obj_set_style_text_align(ui.hdmi_label, LV_TEXT_ALIGN_CENTER, 0);

    ui.hdmi_img = lv_img_create(ui.container);
    lv_img_set_src(ui.hdmi_img, data->is_connect_hdmi ? &icon_hdmi : &icon_hdmi_dis);
    lv_obj_set_pos(ui.hdmi_img, 225, 40);
    lv_obj_set_size(ui.hdmi_img, 36, 30);

    view_net_dis_init(ui.container);

    register_page_gesture(ui.container);
    register_page_gesture(view_network_info_get_swipe_area());
    register_page_gesture(ui.usb_label);
    register_page_gesture(ui.usb_img);
    register_page_gesture(ui.hdmi_label);
    register_page_gesture(ui.hdmi_img);

    lv_obj_t * vndc = get_view_net_dis_container();
    register_page_gesture(vndc);

    NetworkInfoState net_state;
    build_network_state(&net_state, data);
    view_network_info_apply(&net_state);
    view_net_dis_set_visible(!network_info_is_online(&net_state, data));
}

void main_page_update(void)
{
    GlobalData* data = page_get_global_data();
    NetworkInfoState net_state;

    build_network_state(&net_state, data);
    view_network_info_apply(&net_state);

    if(data->is_connect_usb) {
        lv_obj_set_style_text_color(ui.usb_label, lv_color_hex(0x10B981), 0);
        lv_img_set_src(ui.usb_img, &icon_usb);
    } else {
        lv_obj_set_style_text_color(ui.usb_label, lv_color_hex(0xDC2626), 0);
        lv_img_set_src(ui.usb_img, &icon_usb_dis);
    }

    if(data->is_connect_hdmi) {
        lv_obj_set_style_text_color(ui.hdmi_label, lv_color_hex(0x10B981), 0);
        lv_img_set_src(ui.hdmi_img, &icon_hdmi);
    } else {
        lv_obj_set_style_text_color(ui.hdmi_label, lv_color_hex(0xDC2626), 0);
        lv_img_set_src(ui.hdmi_img, &icon_hdmi_dis);
    }

    view_net_dis_set_visible(!network_info_is_online(&net_state, data));
    update_view_data();
}

void main_page_cleanup(void)
{
}
