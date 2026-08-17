#ifndef SETTINGS_REBOOT_PAGE_H
#define SETTINGS_REBOOT_PAGE_H

#include "lvgl/lvgl.h"
#include "../components/view_hold_action.h"

typedef struct {
    lv_obj_t * container;
    ViewHoldAction hold;
} SettingsRebootPageUI;

void settings_reboot_page_init(lv_obj_t * container);
void settings_reboot_page_update(void);
void settings_reboot_page_cleanup(void);

#endif
