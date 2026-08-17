#ifndef SETTINGS_FACTORY_RESET_PAGE_H
#define SETTINGS_FACTORY_RESET_PAGE_H

#include "lvgl/lvgl.h"
#include "../components/view_hold_action.h"

typedef struct {
    lv_obj_t * container;
    lv_obj_t * confirm_panel;
    lv_obj_t * warn_label;
    lv_obj_t * continue_btn;
    lv_obj_t * continue_label;
    ViewHoldAction hold;
} SettingsFactoryResetPageUI;

void settings_factory_reset_page_init(lv_obj_t * container);
void settings_factory_reset_page_update(void);
void settings_factory_reset_page_cleanup(void);

/* Call on page_switch enter so confirm is shown again every visit. */
void settings_factory_reset_page_request_confirm(void);

#endif
