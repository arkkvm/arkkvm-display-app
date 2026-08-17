#include "settings_display_page.h"
#include <stdbool.h>
#include "../components/view_navigation.h"
#include "../core/config_manager.h"
#include "../core/data_manager.h"
#include "../core/page_manager.h"
#include "../utils/font_util.h"

static SettingsDisplayPageUI ui;
static lv_obj_t * current_parent = NULL;

static lv_style_t style_radio;

/* Figma 975:52 — content rows start at y=64, row h=58, label pad_y≈16→17 */
#define DISPLAY_TITLE_Y   81
#define DISPLAY_NORMAL_Y  139
#define DISPLAY_UPSIDE_Y  197


static void radio_btn_event_handler(lv_event_t * e)
{

    GlobalData * page_data  =  page_get_global_data();

    lv_obj_t * btn = lv_event_get_target(e);

    // Determine which button was clicked
    bool is_normal = (btn == ui.normal_btn);

    // Set mutually exclusive checked state on both buttons
    lv_obj_add_state(ui.normal_btn, is_normal ? LV_STATE_CHECKED : LV_STATE_DEFAULT);
    lv_obj_add_state(ui.upside_down_btn, is_normal ? LV_STATE_DEFAULT : LV_STATE_CHECKED);
    if(is_normal) {
        lv_obj_clear_state(ui.upside_down_btn, LV_STATE_CHECKED);
    } else {
        lv_obj_clear_state(ui.normal_btn, LV_STATE_CHECKED);
    }
    // Apply screen rotation from selection
    lv_disp_set_rotation(lv_obj_get_disp(lv_scr_act()), is_normal ? LV_DISP_ROT_90 : LV_DISP_ROT_270);

    page_data->orientation = is_normal ? 1 : 0;
    if(config_update_from_global(page_data) > 0) {
        data_manager_reset_poll_timer();
    }
    
 
}

void settings_display_page_init(lv_obj_t * parent)
{
    current_parent = parent;

    ui.container = are_create_view(parent);
    lv_obj_set_size(ui.container, LV_PCT(100), LV_PCT(100));

    view_navigation_init(ui.container, "Settings", "Display");
    lv_obj_set_style_bg_color(ui.container, lv_color_hex(0x000000), LV_PART_MAIN);

    ui.title_label = lv_label_create(ui.container);
    lv_label_set_text(ui.title_label, "Display Orientation");
    lv_obj_set_style_text_color(ui.title_label, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    lv_obj_set_style_text_font(ui.title_label,
        font_sls(FS_FONT_REGULAR_20) , LV_PART_MAIN);
    lv_obj_set_pos(ui.title_label, 24, DISPLAY_TITLE_Y);

    lv_style_init(&style_radio);
    lv_style_set_radius(&style_radio, LV_RADIUS_CIRCLE);

    ui.normal_btn = lv_checkbox_create(ui.container);
    lv_checkbox_set_text(ui.normal_btn, "Normal");
    lv_obj_set_style_text_font(ui.normal_btn, font_sls(FS_FONT_REGULAR_20), LV_PART_MAIN);
    lv_obj_set_style_text_color(ui.normal_btn, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    lv_obj_set_pos(ui.normal_btn, 24, DISPLAY_NORMAL_Y);
    lv_obj_add_flag(ui.normal_btn, LV_OBJ_FLAG_EVENT_BUBBLE);
    lv_obj_add_event_cb(ui.normal_btn, radio_btn_event_handler, LV_EVENT_CLICKED, NULL);

    ui.upside_down_btn = lv_checkbox_create(ui.container);
    lv_checkbox_set_text(ui.upside_down_btn, "Upside down");
    lv_obj_set_style_text_font(ui.upside_down_btn,font_sls(FS_FONT_REGULAR_20), LV_PART_MAIN);
    lv_obj_set_style_text_color(ui.upside_down_btn, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    lv_obj_set_pos(ui.upside_down_btn, 24, DISPLAY_UPSIDE_Y);
    lv_obj_add_flag(ui.upside_down_btn, LV_OBJ_FLAG_EVENT_BUBBLE);
    lv_obj_add_event_cb(ui.upside_down_btn, radio_btn_event_handler, LV_EVENT_CLICKED, NULL);

    lv_obj_add_style(ui.normal_btn, &style_radio, LV_PART_INDICATOR);
    lv_obj_add_style(ui.upside_down_btn, &style_radio, LV_PART_INDICATOR);

    settings_display_page_update();
}

void settings_display_page_update(void)
{
    GlobalData * page_data  =  page_get_global_data(); 
    

    lv_obj_add_state(ui.normal_btn, page_data->orientation == 1 ? LV_STATE_CHECKED : LV_STATE_DEFAULT);
    lv_obj_add_state(ui.upside_down_btn, page_data->orientation == 0? LV_STATE_CHECKED  : LV_STATE_DEFAULT);  
    
    if(page_data->orientation == 0) { 
         lv_obj_clear_state(ui.normal_btn, LV_STATE_CHECKED); 
    } else { 
        lv_obj_clear_state(ui.upside_down_btn, LV_STATE_CHECKED);
    }
}

void settings_display_page_cleanup(void)
{
    if(current_parent) {
        lv_obj_del(ui.container);
        current_parent = NULL;
    }
}
