#include "view_util.h"

lv_obj_t * are_create_view(lv_obj_t * parent)
{

    lv_obj_t * view = lv_obj_create(parent);

    lv_obj_set_style_bg_color(view, lv_color_hex(0x000000), LV_PART_MAIN);
    // Clear default styles that may affect layout
    lv_obj_set_style_border_width(view, 0, LV_PART_MAIN); // Remove border
    lv_obj_set_style_pad_all(view, 0, LV_PART_MAIN);
    // lv_obj_clear_flag(view, LV_OBJ_FLAG_SCROLLABLE); // Disable scrolling
    lv_obj_set_style_radius(view, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    return view;
}