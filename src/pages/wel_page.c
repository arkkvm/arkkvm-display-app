#include "wel_page.h"
#include "../../lvgl/lvgl.h"
#include <stdio.h>
#include <string.h>
#include "../utils/font_util.h"

static WelPageUI ui;

LV_IMG_DECLARE(icon_wel_logo);

void wel_page_init(lv_obj_t * container)
{

    ui.container = container;

    // Create icon
    lv_obj_t * icon = lv_img_create(container);
    lv_img_set_src(icon, &icon_wel_logo);
    lv_obj_set_pos(icon, 62, 100);
    lv_obj_set_size(icon, 160, 30);

    // Create status label
    ui.state_label = lv_label_create(container);
    lv_label_set_text(ui.state_label, "Starting...");
    lv_obj_set_style_text_font(ui.state_label, font_sls(FS_FONT_REGULAR_14) , 0);
    // lv_obj_set_style_text_font(ui.state_label, &lv_font_montserrat_14, 0);
    lv_obj_set_style_text_color(ui.state_label, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    // Position label: centered horizontally, 184px from top
    lv_obj_align(ui.state_label, LV_ALIGN_TOP_MID, 0, 184);
}
void wel_page_update(void)
{}
void wel_page_cleanup(void)
{}