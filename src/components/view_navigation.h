#ifndef VIEW_NAVIGATION_H
#define VIEW_NAVIGATION_H

#include "../../lvgl/lvgl.h"
#include "../utils/view_util.h"

#define NAV_HEIGHT 63
typedef struct {
    lv_obj_t* container;
    lv_obj_t* back_btn;
    lv_obj_t* back_icon;
    lv_obj_t* back_label;
    lv_obj_t* title_label;
} NavigationView;

// Initialize navigation bar
void view_navigation_init(lv_obj_t* parent, const char* back_text, const char* title);

// Set back button visibility
void view_navigation_set_back_enabled(bool enabled);

// Set title text
void view_navigation_set_title(const char* title);

#endif