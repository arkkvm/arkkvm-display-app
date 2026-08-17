#ifndef SETTINGS_PAGE_H
#define SETTINGS_PAGE_H

#include "lvgl/lvgl.h"
#include "../core/page_manager.h"

typedef struct {
    lv_obj_t* container;
    lv_obj_t* menu_list;
    lv_obj_t* display_btn;
    lv_obj_t* about_btn;
    lv_obj_t* network_reset_btn;
    lv_obj_t* reboot_btn;
    lv_obj_t* factory_reset_btn;
} SettingsPageUI;

void settings_page_init(lv_obj_t* container);
void settings_page_update(void);
void settings_page_cleanup(void);
/* Call when navigating to Settings so the menu opens at the top. */
void settings_page_request_scroll_top(void);

#endif
 


 

 
