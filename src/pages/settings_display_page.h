#ifndef SETTINGS_DISPLAY_PAGE_H
#define SETTINGS_DISPLAY_PAGE_H

#include "lvgl/lvgl.h"

typedef struct {
    lv_obj_t *container;
    lv_obj_t *title_label;
    lv_obj_t *normal_btn;
    lv_obj_t *upside_down_btn;
} SettingsDisplayPageUI;


void settings_display_page_init(lv_obj_t *parent);
void settings_display_page_update(void);
void settings_display_page_cleanup(void);

#endif