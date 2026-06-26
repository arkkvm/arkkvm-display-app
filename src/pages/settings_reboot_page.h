#ifndef SETTINGS_REBOOT_PAGE_H
#define SETTINGS_REBOOT_PAGE_H

#include "lvgl/lvgl.h"

typedef struct {
    lv_obj_t *container;
    lv_obj_t *reboot_icon;    
    lv_obj_t *reboot_label;   
} SettingsRebootPageUI;

void settings_reboot_page_init(lv_obj_t* container);
void settings_reboot_page_update(void);
void settings_reboot_page_cleanup(void);


#endif
