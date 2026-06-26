// components/view_navigation.c
#include "view_navigation.h"
#include "../core/page_manager.h"
#include <string.h>
#include "../utils/font_util.h"
// Icon declarations (defined elsewhere in the project)
LV_IMG_DECLARE(icon_back);

static NavigationView ui;

// Back button click handler
static void back_btn_handler(lv_event_t * e)
{
    (void)e;     // Suppress unused parameter warning
    page_back(); // Navigate back via page manager
}

void view_navigation_init(lv_obj_t * parent, const char * back_text, const char * title)
{
    // Create container

    ui.container = are_create_view(parent);
    lv_obj_set_size(ui.container, LV_PCT(100), NAV_HEIGHT);
    lv_obj_clear_flag(ui.container, LV_OBJ_FLAG_SCROLLABLE); // Disable scrolling

    // Left panel (clickable back area)
    lv_obj_t * left_panel = are_create_view(ui.container);
    lv_obj_set_size(left_panel, LV_PCT(50), NAV_HEIGHT);
    lv_obj_add_flag(left_panel, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_event_cb(left_panel, back_btn_handler, LV_EVENT_CLICKED, NULL);

    // Back icon
    ui.back_icon = lv_img_create(left_panel);
    lv_img_set_src(ui.back_icon, &icon_back);
    lv_obj_set_pos(ui.back_icon, 24, 25);
    lv_obj_set_size(ui.back_icon, 16, 16);

    // Back label
    ui.back_label = lv_label_create(left_panel);
    lv_label_set_text(ui.back_label, back_text);
    lv_obj_set_style_text_color(ui.back_label, lv_color_hex(0x60A5FA), LV_PART_MAIN);
    lv_obj_set_style_text_font(ui.back_label,font_sls(FS_FONT_REGULAR_16),LV_PART_MAIN);
    lv_obj_set_pos(ui.back_label, 44, 27);

    // Right panel (title area)
    lv_obj_t * right_panel = are_create_view(ui.container);
    lv_obj_set_size(right_panel, LV_PCT(50), NAV_HEIGHT);
    lv_obj_set_pos(right_panel, LV_PCT(50), 0);

    // Page title
    ui.title_label = lv_label_create(right_panel);
    lv_label_set_text(ui.title_label, title);
    lv_obj_set_style_text_color(ui.title_label, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    lv_obj_set_style_text_font(ui.title_label,font_sls(FS_FONT_REGULAR_16) , LV_PART_MAIN);
    lv_obj_align(ui.title_label, LV_ALIGN_TOP_RIGHT, -24, 27);

    lv_obj_t * line = are_create_view(ui.container);
    lv_obj_set_size(line, 300, 1); // width=300, height=1
    lv_obj_set_pos(line, 0, NAV_HEIGHT - 2);
    lv_obj_set_style_bg_color(line, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    // lv_obj_clear_flag(line, LV_OBJ_FLAG_CLICKABLE); // Disable click
    lv_obj_move_foreground(line);
}

void view_navigation_set_back_enabled(bool enabled)
{
    if(enabled) {
        lv_obj_clear_flag(ui.back_btn, LV_OBJ_FLAG_HIDDEN);
    } else {
        lv_obj_add_flag(ui.back_btn, LV_OBJ_FLAG_HIDDEN);
    }
}

void view_navigation_set_title(const char * title)
{
    lv_label_set_text(ui.title_label, title);
}