#ifndef MAIN_PAGE_H
#define MAIN_PAGE_H

#include "../core/page_manager.h"
#include <stdbool.h>

typedef struct {
    lv_obj_t* container;
    lv_obj_t* logo_img;
    lv_obj_t* usb_label;
    lv_obj_t* usb_img;
    lv_obj_t* hdmi_label;
    lv_obj_t* hdmi_img;
} MainPageUI;

void main_page_init(lv_obj_t* container);
void main_page_update(void);
void main_page_cleanup(void);

#endif
