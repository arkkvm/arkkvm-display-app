#ifndef SETTINGS_NETWORK_RESET_PAGE_H
#define SETTINGS_NETWORK_RESET_PAGE_H

#include "lvgl/lvgl.h"
#include "../components/view_hold_action.h"

typedef struct {
    lv_obj_t * container;
    ViewHoldAction hold;
} SettingsNetworkResetPageUI;

void settings_network_reset_page_init(lv_obj_t * container);
void settings_network_reset_page_update(void);
void settings_network_reset_page_cleanup(void);

#endif
