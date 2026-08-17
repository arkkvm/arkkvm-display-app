#include "settings_page.h"
#include "../components/view_navigation.h"
#include "../core/page_manager.h"
#include "../utils/font_util.h"
#include "../utils/view_util.h"

#define MENU_ROW_H       64
#define MENU_LABEL_PAD_X 24
#define MENU_PRESS_BG    0x262626

static SettingsPageUI ui;
static bool s_scroll_to_top;

static void menu_btn_handler(lv_event_t * e)
{
    lv_obj_t * btn = lv_event_get_target(e);

    if(btn == ui.display_btn) {
        PAGE_SWITCH(PAGE_SETTINGS_DISPLAY);
    } else if(btn == ui.about_btn) {
        PAGE_SWITCH(PAGE_SETTINGS_ABOUT);
    } else if(btn == ui.reboot_btn) {
        PAGE_SWITCH(PAGE_SETTINGS_REBOOT);
    } else if(btn == ui.network_reset_btn) {
        PAGE_SWITCH(PAGE_SETTINGS_NETWORK_RESET);
    } else if(btn == ui.factory_reset_btn) {
        PAGE_SWITCH(PAGE_SETTINGS_FACTORY_RESET);
    }
}

static lv_obj_t * create_menu_row(lv_obj_t * parent, const char * text, uint32_t color,
                                  int index)
{
    lv_obj_t * btn = lv_btn_create(parent);
    lv_obj_set_style_radius(btn, 0, LV_PART_MAIN);
    lv_obj_set_size(btn, LV_PCT(100), MENU_ROW_H);
    lv_obj_set_pos(btn, 0, index * MENU_ROW_H);
    lv_obj_set_style_bg_color(btn, lv_color_hex(0x000000), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(btn, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_set_style_bg_color(btn, lv_color_hex(MENU_PRESS_BG), LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_shadow_width(btn, 0, LV_PART_MAIN);
    lv_obj_set_style_border_width(btn, 0, LV_PART_MAIN);
    lv_obj_set_style_pad_all(btn, 0, LV_PART_MAIN);
    lv_obj_add_event_cb(btn, menu_btn_handler, LV_EVENT_CLICKED, NULL);

    lv_obj_t * label = lv_label_create(btn);
    lv_label_set_text(label, text);
    lv_obj_set_style_text_color(label, lv_color_hex(color), LV_PART_MAIN);
    lv_obj_set_style_text_font(label, font_sls(FS_FONT_REGULAR_20), LV_PART_MAIN);
    lv_obj_align(label, LV_ALIGN_LEFT_MID, MENU_LABEL_PAD_X, 0);

    return btn;
}

void settings_page_init(lv_obj_t * container)
{
    ui.container = container;

    lv_obj_set_style_bg_color(ui.container, lv_color_hex(0x000000), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(ui.container, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_set_size(ui.container, LV_PCT(100), LV_PCT(100));
    lv_obj_clear_flag(ui.container, LV_OBJ_FLAG_SCROLLABLE);

    /* Fixed nav (FLOATING) — divider does not scroll with the list. */
    view_navigation_init(ui.container, "Home", "Settings");

    ui.menu_list = are_create_view(ui.container);
    lv_obj_set_size(ui.menu_list, LV_PCT(100), 240 - NAV_HEIGHT);
    lv_obj_set_pos(ui.menu_list, 0, NAV_HEIGHT);
    lv_obj_set_style_bg_color(ui.menu_list, lv_color_hex(0x000000), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(ui.menu_list, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_set_style_pad_all(ui.menu_list, 0, LV_PART_MAIN);
    lv_obj_set_style_border_width(ui.menu_list, 0, LV_PART_MAIN);
    lv_obj_add_flag(ui.menu_list, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_scroll_dir(ui.menu_list, LV_DIR_VER);
    lv_obj_set_scrollbar_mode(ui.menu_list, LV_SCROLLBAR_MODE_OFF);
    lv_obj_clear_flag(ui.menu_list, LV_OBJ_FLAG_SCROLL_CHAIN);

    /* Figma 863:1243: Display / About / Reboot / Restore Network / Factory Reset */
    ui.display_btn = create_menu_row(ui.menu_list, "Display", 0xFFFFFF, 0);
    ui.about_btn = create_menu_row(ui.menu_list, "About", 0xFFFFFF, 1);
    ui.reboot_btn = create_menu_row(ui.menu_list, "Reboot", 0xDC2626, 2);
    ui.network_reset_btn = create_menu_row(ui.menu_list, "Restore Network", 0xDC2626, 3);
    ui.factory_reset_btn = create_menu_row(ui.menu_list, "Factory Reset", 0xDC2626, 4);
}

void settings_page_request_scroll_top(void)
{
    s_scroll_to_top = true;
}

void settings_page_update(void)
{
    /* Only snap on navigation enter — not on page_update_all / data refresh. */
    if(s_scroll_to_top && ui.menu_list) {
        lv_obj_scroll_to_y(ui.menu_list, 0, LV_ANIM_OFF);
        s_scroll_to_top = false;
    }
}

void settings_page_cleanup(void)
{
}
