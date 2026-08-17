#ifndef VIEW_NAVIGATION_H
#define VIEW_NAVIGATION_H

#include "../../lvgl/lvgl.h"
#include "../utils/view_util.h"

/* Figma settings chrome: content starts at y=64, divider on nav bottom edge */
#define NAV_HEIGHT 64
typedef struct {
    lv_obj_t* container;
    lv_obj_t* back_btn;
    lv_obj_t* back_icon;
    lv_obj_t* back_label;
    lv_obj_t* title_label;
    lv_obj_t* divider;
} NavigationView;

// Initialize navigation bar
void view_navigation_init(lv_obj_t* parent, const char* back_text, const char* title);

// Set back button visibility
void view_navigation_set_back_enabled(bool enabled);

// Set title text
void view_navigation_set_title(const char* title);

/* Bring nav above later-created siblings so Back stays clickable. */
void view_navigation_raise(void);

#endif