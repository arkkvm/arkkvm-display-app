#ifndef SETTINGS_PAGE_H
#define SETTINGS_PAGE_H

#include "lvgl/lvgl.h"
#include "../core/page_manager.h"

typedef struct {
    lv_obj_t* container;
    lv_obj_t* display_btn;
    lv_obj_t* about_btn;
    lv_obj_t* reboot_btn;
} SettingsPageUI;

void settings_page_init(lv_obj_t* container);
void settings_page_update(void);
void settings_page_cleanup(void);

#endif
 


 

 
