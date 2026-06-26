#include "settings_about_page.h"
#include "../components/view_navigation.h"
#include "../utils/font_util.h"

static SettingsAboutPageUI ui;
static lv_obj_t * current_parent = NULL;

void settings_about_page_init(lv_obj_t * parent)
{
    current_parent = parent;

    GlobalData * data = page_get_global_data();
    ui.container = are_create_view(parent);
    lv_obj_set_size(ui.container, LV_PCT(100), LV_PCT(100));
    lv_obj_set_style_bg_color(ui.container, lv_color_hex(0x000000), LV_PART_MAIN);

    view_navigation_init(ui.container, "Back", "About");

    ui.content_panel = are_create_view(ui.container);
    lv_obj_set_size(ui.content_panel, LV_PCT(100), 240 - NAV_HEIGHT);
    lv_obj_set_pos(ui.content_panel, 0, 59);
    lv_obj_set_flex_flow(ui.content_panel, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_style_bg_opa(ui.content_panel, LV_OPA_TRANSP, LV_PART_MAIN); // Transparent background

    lv_obj_set_style_pad_left(ui.content_panel, 24, LV_PART_MAIN); // Left padding 24px
    lv_obj_set_style_pad_top(ui.content_panel, 16, LV_PART_MAIN);  // Top padding 16px
    lv_obj_set_style_pad_row(ui.content_panel, 8, LV_PART_MAIN);   // Row spacing 8px

    ui.app_version_label = lv_label_create(ui.content_panel);
    lv_label_set_text(ui.app_version_label, "ArkKVM app");
    lv_obj_set_style_text_font(ui.app_version_label, 
        font_sls(FS_FONT_REGULAR_16), LV_PART_MAIN);
    lv_obj_set_style_text_color(ui.app_version_label, lv_color_white(), LV_PART_MAIN);

    ui.app_version_value = lv_label_create(ui.content_panel);
    lv_label_set_text(ui.app_version_value, data->app_version);
    lv_obj_set_style_text_font(ui.app_version_value, font_sls(FS_FONT_REGULAR_14), LV_PART_MAIN);
    lv_obj_set_style_text_color(ui.app_version_value, lv_color_hex(0xA3A3A3), LV_PART_MAIN);
    lv_obj_set_style_pad_bottom(ui.app_version_value, 16, LV_PART_MAIN); // Extra bottom spacing


    ui.os_version_label = lv_label_create(ui.content_panel);
    lv_label_set_text(ui.os_version_label, "ArkKVM OS");
    lv_obj_set_style_text_font(ui.os_version_label, font_sls(FS_FONT_REGULAR_16), LV_PART_MAIN);
    lv_obj_set_style_text_color(ui.os_version_label, lv_color_white(), LV_PART_MAIN);

    ui.os_version_value = lv_label_create(ui.content_panel);
    lv_label_set_text(ui.os_version_value, data->os_version);
    lv_obj_set_style_text_font(ui.os_version_value, font_sls(FS_FONT_REGULAR_14), LV_PART_MAIN);
    lv_obj_set_style_text_color(ui.os_version_value, lv_color_hex(0xA3A3A3), LV_PART_MAIN);
    lv_obj_set_style_pad_bottom(ui.os_version_value, 16, LV_PART_MAIN);


    ui.serial_label = lv_label_create(ui.content_panel);
    lv_label_set_text(ui.serial_label, "Serial Number");
    lv_obj_set_style_text_font(ui.serial_label, font_sls(FS_FONT_REGULAR_16), LV_PART_MAIN);
    lv_obj_set_style_text_color(ui.serial_label, lv_color_white(), LV_PART_MAIN);

    ui.serial_value = lv_label_create(ui.content_panel);
    lv_label_set_text(ui.serial_value, data->serial_number);
    lv_obj_set_style_text_font(ui.serial_value, font_sls(FS_FONT_REGULAR_14), LV_PART_MAIN);
    lv_obj_set_style_text_color(ui.serial_value, lv_color_hex(0xA3A3A3), LV_PART_MAIN);
    lv_obj_set_style_pad_bottom(ui.serial_value, 16, LV_PART_MAIN);

    lv_obj_t * none_view = are_create_view(ui.content_panel);
    lv_obj_set_height(none_view, 29); // Spacer so last label is not clipped

    lv_obj_set_scrollbar_mode(ui.content_panel, LV_SCROLLBAR_MODE_AUTO);
    lv_obj_set_scrollbar_mode(ui.content_panel, LV_SCROLLBAR_MODE_OFF);
}

void settings_about_page_update(void)
{
     
    // lv_label_set_text(ui.app_version_value, config_manager_get_app_version());
    // lv_label_set_text(ui.serial_value, config_manager_get_serial());
}

void settings_about_page_cleanup(void)
{
    if(current_parent) {
        lv_obj_del(ui.container);
        current_parent = NULL;
    }
}