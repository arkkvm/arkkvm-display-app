// components/view_navigation.c
#include "view_navigation.h"
#include "../core/page_manager.h"
#include <string.h>
#include "../utils/font_util.h"

LV_IMG_DECLARE(icon_back);

/* Leave room for back hit area (x=0..120) + 8px gap before title scrolls. */
#define NAV_TITLE_MAX_W 132
/*
 * Absolute Y from the previously accepted layout (icon 2px above label top).
 * icon_back is 16×17 with transparent padding — MID/flex center makes the
 * chevron look lower than the Back glyphs (worse with Regular 20 line box).
 */
#define NAV_BACK_ICON_X  24
#define NAV_BACK_ICON_Y  25
#define NAV_BACK_LABEL_X 44
#define NAV_BACK_LABEL_Y 27
#define NAV_TITLE_Y      27

static NavigationView ui;

static void back_btn_handler(lv_event_t * e)
{
    (void)e;
    page_back();
}

void view_navigation_init(lv_obj_t * parent, const char * back_text, const char * title)
{
    ui.container = are_create_view(parent);
    lv_obj_set_size(ui.container, LV_PCT(100), NAV_HEIGHT);
    lv_obj_set_pos(ui.container, 0, 0);
    lv_obj_clear_flag(ui.container, LV_OBJ_FLAG_SCROLLABLE);
    /* Keep nav above scrolling siblings so the divider never redraw-flickers. */
    lv_obj_add_flag(ui.container, LV_OBJ_FLAG_FLOATING);
    lv_obj_move_foreground(ui.container);
    lv_obj_set_style_bg_opa(ui.container, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_set_style_bg_color(ui.container, lv_color_hex(0x000000), LV_PART_MAIN);

    /* Clickable back hit area — wide enough for "Settings" (Figma ~96px row + x=24). */
    lv_obj_t * left_panel = are_create_view(ui.container);
    lv_obj_set_size(left_panel, 128, NAV_HEIGHT);
    lv_obj_set_style_bg_opa(left_panel, LV_OPA_TRANSP, LV_PART_MAIN);
    lv_obj_clear_flag(left_panel, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_add_flag(left_panel, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_event_cb(left_panel, back_btn_handler, LV_EVENT_CLICKED, NULL);

    ui.back_icon = lv_img_create(left_panel);
    lv_img_set_src(ui.back_icon, &icon_back);
    /* Display 16×16; source asset is 16×17 with transparent pad. */
    lv_obj_set_size(ui.back_icon, 16, 16);
    lv_obj_set_pos(ui.back_icon, NAV_BACK_ICON_X, NAV_BACK_ICON_Y);
    lv_obj_clear_flag(ui.back_icon, LV_OBJ_FLAG_CLICKABLE);

    ui.back_label = lv_label_create(left_panel);
    lv_label_set_text(ui.back_label, back_text);
    lv_obj_set_style_text_color(ui.back_label, lv_color_hex(0x60A5FA), LV_PART_MAIN);
    lv_obj_set_style_text_font(ui.back_label, font_sls(FS_FONT_REGULAR_20), LV_PART_MAIN);
    lv_obj_set_pos(ui.back_label, NAV_BACK_LABEL_X, NAV_BACK_LABEL_Y);
    lv_obj_add_flag(ui.back_label, LV_OBJ_FLAG_EVENT_BUBBLE);

    /*
     * Title on full-width nav, right-aligned. Long titles (e.g. Restore Network)
     * scroll circularly within a capped width so they never cover Back.
     */
    ui.title_label = lv_label_create(ui.container);
    lv_label_set_text(ui.title_label, title);
    lv_obj_set_style_text_color(ui.title_label, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    lv_obj_set_style_text_font(ui.title_label, font_sls(FS_FONT_REGULAR_20), LV_PART_MAIN);
    lv_obj_set_width(ui.title_label, NAV_TITLE_MAX_W);
    lv_label_set_long_mode(ui.title_label, LV_LABEL_LONG_SCROLL_CIRCULAR);
    lv_obj_set_style_text_align(ui.title_label, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN);
    lv_obj_align(ui.title_label, LV_ALIGN_TOP_RIGHT, -24, NAV_TITLE_Y);

    ui.divider = are_create_view(ui.container);
    lv_obj_set_size(ui.divider, LV_PCT(100), 1);
    lv_obj_set_pos(ui.divider, 0, NAV_HEIGHT - 1);
    lv_obj_set_style_bg_color(ui.divider, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(ui.divider, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_clear_flag(ui.divider, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_move_foreground(ui.divider);
}

void view_navigation_raise(void)
{
    if(ui.container) {
        lv_obj_move_foreground(ui.container);
    }
}

void view_navigation_set_back_enabled(bool enabled)
{
    if(ui.back_icon == NULL) {
        return;
    }
    if(enabled) {
        lv_obj_clear_flag(ui.back_icon, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(ui.back_label, LV_OBJ_FLAG_HIDDEN);
    } else {
        lv_obj_add_flag(ui.back_icon, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(ui.back_label, LV_OBJ_FLAG_HIDDEN);
    }
}

void view_navigation_set_title(const char * title)
{
    if(ui.title_label) {
        lv_label_set_text(ui.title_label, title);
        lv_obj_set_width(ui.title_label, NAV_TITLE_MAX_W);
        lv_label_set_long_mode(ui.title_label, LV_LABEL_LONG_SCROLL_CIRCULAR);
        lv_obj_align(ui.title_label, LV_ALIGN_TOP_RIGHT, -24, NAV_TITLE_Y);
    }
}
