
#include "view_net_dis.h"
#include "../core/page_manager.h"
#include "../utils/font_util.h"

typedef struct {
    lv_obj_t* container;
    lv_obj_t* icon;
    lv_obj_t* label;
    lv_obj_t* mac_label;
    lv_obj_t* mac_value;
    
} NetDisableView;

static NetDisableView ui;
LV_IMG_DECLARE(icon_net_dis);

void view_net_dis_init(lv_obj_t* parent) {

    ui.container = lv_obj_create(parent);
    lv_obj_set_size(ui.container, LV_PCT(100), LV_PCT(100));
    lv_obj_set_style_border_width(ui.container , 0, LV_PART_MAIN); 
    lv_obj_set_style_pad_all(ui.container , 0, LV_PART_MAIN); 
    lv_obj_clear_flag(ui.container , LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_pos(ui.container, 0, 0);
    lv_obj_set_style_bg_color(ui.container, lv_color_hex(0x000000), LV_PART_MAIN);
    lv_obj_clear_flag(ui.container, LV_OBJ_FLAG_SCROLLABLE); 
    lv_obj_set_style_pad_all(ui.container, 0, LV_PART_MAIN); 
    // lv_obj_set_style_margin_all(ui.container, 0, LV_PART_MAIN); 



    ui.icon = lv_img_create(ui.container);
    lv_img_set_src(ui.icon, &icon_net_dis); 
    lv_obj_set_pos(ui.icon, 124, 54);


    ui.label = lv_label_create(ui.container);
    lv_label_set_text(ui.label, "Please insert RJ45 cable and connect to network");
    lv_obj_set_style_text_color(ui.label, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    lv_obj_set_style_text_font(ui.label,
        font_sls(FS_FONT_REGULAR_14), 0); 
    lv_obj_set_size(ui.label, 200, 40);
    lv_obj_set_pos(ui.label, 42, 102);
    lv_obj_set_style_text_align(ui.label, LV_TEXT_ALIGN_CENTER, 0);
   

    ui.mac_label = lv_label_create(ui.container);
    lv_label_set_text(ui.mac_label, "Device MAC");
    lv_obj_set_style_text_color(ui.mac_label, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    lv_obj_set_style_text_font(ui.mac_label,font_sls(FS_FONT_SEMIBOLD_14)   , LV_PART_MAIN);
 
    lv_obj_set_pos(ui.mac_label, 42, 170);
    lv_obj_set_size(ui.mac_label, 200, 14);
    lv_obj_set_style_text_align(ui.mac_label, LV_TEXT_ALIGN_CENTER, 0);

    GlobalData* data = page_get_global_data();
    ui.mac_value = lv_label_create(ui.container);
    lv_label_set_text(ui.mac_value, data->mac);
    lv_obj_set_style_text_color(ui.mac_value, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    lv_obj_set_style_text_font(ui.mac_value, font_sls(FS_FONT_SEMIBOLD_14)  , LV_PART_MAIN); 
    lv_obj_set_pos(ui.mac_value, 42, 184);
    lv_obj_set_size(ui.mac_value, 200, 14);
    lv_obj_set_style_text_align(ui.mac_value, LV_TEXT_ALIGN_CENTER, 0);

    
    lv_obj_add_flag(ui.container, LV_OBJ_FLAG_HIDDEN);

}

void view_net_dis_set_visible(bool visible) {
    if (visible) {
        lv_obj_clear_flag(ui.container, LV_OBJ_FLAG_HIDDEN);
    } else {
        lv_obj_add_flag(ui.container, LV_OBJ_FLAG_HIDDEN);
    }
}


lv_obj_t* get_view_net_dis_container(void) {
    return ui.container;
}

void update_view_data(){
    GlobalData* data = page_get_global_data();
    lv_label_set_text(ui.mac_value, data->mac);
}