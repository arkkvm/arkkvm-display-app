#ifndef SETTINGS_ABOUT_PAGE_H
#define SETTINGS_ABOUT_PAGE_H

#include "lvgl/lvgl.h"
#include "../core/page_manager.h"

typedef struct {
    lv_obj_t *container;
    lv_obj_t *content_panel;
    lv_obj_t *app_version_label;
    lv_obj_t *app_version_value;
    lv_obj_t *os_version_label;
    lv_obj_t *os_version_value;
    lv_obj_t *serial_label;
    lv_obj_t *serial_value;
} SettingsAboutPageUI;

void settings_about_page_init(lv_obj_t* container);
void settings_about_page_update(void);
void settings_about_page_cleanup(void);



#endif
