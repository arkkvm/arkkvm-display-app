#include "settings_page.h"
#include "../components/view_navigation.h"
#include "../core/page_manager.h"
#include "../utils/font_util.h"

static SettingsPageUI ui;

// menu event handler
static void menu_btn_handler(lv_event_t * e)
{
    lv_obj_t * btn = lv_event_get_target(e);

 
    // lv_obj_set_style_bg_color(ui.display_btn, lv_color_hex(0x000000), LV_PART_MAIN);
    // lv_obj_set_style_bg_color(ui.about_btn, lv_color_hex(0x000000), LV_PART_MAIN);
 

    if(btn == ui.display_btn){
        PAGE_SWITCH(PAGE_SETTINGS_DISPLAY);
    }else if(btn == ui.about_btn){
        PAGE_SWITCH(PAGE_SETTINGS_ABOUT);
    }else   if(btn == ui.reboot_btn){
        PAGE_SWITCH(PAGE_SETTINGS_REBOOT);
    }
    //     lv_obj_set_style_bg_color(btn, lv_color_hex(0xDC2626), LV_PART_MAIN);
 
    //     // execute_command("reboot");
    // }
}

void settings_page_init(lv_obj_t * container)
{
    ui.container = container;

    lv_obj_set_style_bg_color(ui.container, lv_color_hex(0x00FF00), LV_PART_MAIN); 
    lv_obj_set_size(ui.container, LV_PCT(100), LV_PCT(100));
     
    view_navigation_init(ui.container, "Back", "Settings");

   
    int menu_height = (SCREEN_HEIGHT - NAV_HEIGHT) / 3;
 

    // Display btn
    ui.display_btn = lv_btn_create(ui.container);
    lv_obj_set_style_radius(ui.display_btn, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_size(ui.display_btn, LV_PCT(100), menu_height);
    lv_obj_set_pos(ui.display_btn, 0, NAV_HEIGHT);
    lv_obj_set_style_bg_color(ui.display_btn, lv_color_hex(0x000000), LV_PART_MAIN);
    lv_obj_add_event_cb(ui.display_btn, menu_btn_handler, LV_EVENT_CLICKED, NULL);

    lv_obj_t * display_label = lv_label_create(ui.display_btn);
    lv_label_set_text(display_label, "Display");
    lv_obj_set_style_text_color(display_label, lv_color_white(), LV_PART_MAIN);
    lv_obj_set_style_text_font(display_label,font_sls(FS_FONT_REGULAR_16), LV_PART_MAIN);
    lv_obj_align(display_label, LV_ALIGN_LEFT_MID, 10, 0);

    // About btn
    ui.about_btn = lv_btn_create(ui.container);
    lv_obj_set_style_radius(ui.about_btn, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_size(ui.about_btn, LV_PCT(100), menu_height);
    lv_obj_set_pos(ui.about_btn, 0, NAV_HEIGHT + menu_height);
    lv_obj_set_style_bg_color(ui.about_btn, lv_color_hex(0x000000), LV_PART_MAIN);
    lv_obj_add_event_cb(ui.about_btn, menu_btn_handler, LV_EVENT_CLICKED, NULL);

    lv_obj_t * about_label = lv_label_create(ui.about_btn);
    lv_label_set_text(about_label, "About");
    lv_obj_set_style_text_color(about_label, lv_color_white(), LV_PART_MAIN);
    lv_obj_set_style_text_font(about_label,font_sls(FS_FONT_REGULAR_16), LV_PART_MAIN);
    lv_obj_align(about_label, LV_ALIGN_LEFT_MID, 10, 0);

    // Reboot btn
    ui.reboot_btn = lv_btn_create(ui.container);
    lv_obj_set_style_radius(ui.reboot_btn, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_size(ui.reboot_btn, LV_PCT(100), menu_height);
    lv_obj_set_pos(ui.reboot_btn, 0, NAV_HEIGHT + 2 * menu_height);
    lv_obj_set_style_bg_color(ui.reboot_btn, lv_color_hex(0x000000), LV_PART_MAIN);
    lv_obj_add_event_cb(ui.reboot_btn, menu_btn_handler, LV_EVENT_CLICKED, NULL);

    lv_obj_t * reboot_label = lv_label_create(ui.reboot_btn);
    lv_label_set_text(reboot_label, "Reboot");
    lv_obj_set_style_text_color(reboot_label, lv_color_hex(0xDC2626), LV_PART_MAIN);
    lv_obj_set_style_text_font(reboot_label, font_sls(FS_FONT_REGULAR_16), LV_PART_MAIN);
    lv_obj_align(reboot_label, LV_ALIGN_LEFT_MID, 10, 0);
}

void settings_page_update(void)
{

}

void settings_page_cleanup(void)
{

}