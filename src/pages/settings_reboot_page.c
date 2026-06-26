#include "settings_reboot_page.h"
#include "../components/view_navigation.h"

#include "../utils/command.h" 
#include "../utils/font_util.h"



static SettingsRebootPageUI ui;
static lv_obj_t *current_parent = NULL;

LV_IMG_DECLARE(icon_reboot);



static void reboot_event_handler(lv_event_t *e) {
    if (e->code == LV_EVENT_LONG_PRESSED) {
        
        run_command_no_result("reboot");
    }
}

void settings_reboot_page_init(lv_obj_t *parent) {
    current_parent = parent;


    ui.container = are_create_view(parent);
    lv_obj_set_size(ui.container, LV_PCT(100), LV_PCT(100));
    lv_obj_set_style_bg_color(ui.container, lv_color_hex(0x000000), LV_PART_MAIN);

    // Initialize navigation bar (title: "Reboot")
    view_navigation_init(ui.container, "Back", "Reboot");

    // create reboot icon
    ui.reboot_icon = lv_img_create(ui.container);
    lv_img_set_src(ui.reboot_icon, &icon_reboot);  
    lv_obj_set_size(ui.reboot_icon, 118, 118);
    lv_obj_set_pos(ui.reboot_icon, 83, 80);
    lv_obj_add_flag(ui.reboot_icon, LV_OBJ_FLAG_CLICKABLE); 
    
   
    lv_obj_add_event_cb(ui.reboot_icon, reboot_event_handler, LV_EVENT_LONG_PRESSED, NULL);

    // create reboot label
    ui.reboot_label = lv_label_create(ui.container);
    lv_label_set_text(ui.reboot_label, "Press and Hold to Reboot");
    lv_obj_set_style_text_color(ui.reboot_label, lv_color_white(), LV_PART_MAIN);
    lv_obj_set_style_text_font(ui.reboot_label,font_sls(FS_FONT_REGULAR_16), LV_PART_MAIN);

    lv_obj_align(ui.reboot_label, LV_ALIGN_TOP_MID, 0, 206);
}

void settings_reboot_page_update(void) {
 
}

void settings_reboot_page_cleanup(void) {
    if (current_parent) {
        lv_obj_del(ui.container);
        current_parent = NULL;
    }
}