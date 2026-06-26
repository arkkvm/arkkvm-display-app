#include "lvgl/lvgl.h"
#include "src/core/page_manager.h"
#include "src/core/data_manager.h"
#include "src/core/config_manager.h"
#include "src/core/socket_server.h"
#include "src/utils/command.h"
#include "src/utils/debug.h"
#include "src/utils/time_util.h"
#include "lv_drivers/display/fbdev.h"
#include "lv_drivers/indev/evdev.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <stdbool.h>
#include <limits.h>

#define DISP_BUF_SIZE (128 * 1024)

bool g_debug_log = false;

static bool is_init_ui = false;

static void screen_timer_callback(lv_timer_t * t)
{

    DEBUG_LOG("Current time: %lld\n", (long long)get_now_time());
    GlobalData * data = page_get_global_data();

    uint32_t free_time = lv_tick_get() - data->lastTouchTime;
    DEBUG_LOG("[Screen timer] Idle time: %llds (dark screen threshold: %ds, sleep threshold: %ds)\n",
              (long long)(free_time / 1000), data->dark_screen_time, data->sleep_time);
    if(!data->is_dark_screen && data->dark_screen_time != 0 && free_time >= (data->dark_screen_time * 1000)) {
        DEBUG_LOG(">>> Dark screen triggered (idle for %us)\n", data->dark_screen_time);
        data->is_dark_screen = true;
        update_screen_luminance();
    }

    if(!data->is_sleep && data->sleep_time != 0 && free_time >= (data->sleep_time * 1000)) {
        data->is_sleep = true;
        DEBUG_LOG(">>> Sleep triggered (idle for %us)\n", data->sleep_time);
        update_screen_luminance();
    }

    
}
static void timer_callback(lv_timer_t * t)
{
    data_manager_update();
}

void my_touch_feedback_cb(lv_indev_drv_t * drv, lv_event_t e)
{

    GlobalData * data = page_get_global_data();

    DEBUG_LOG("[Touch event] dark_screen=%d, sleep=%d\n", data->is_dark_screen, data->is_sleep);
    if(data->is_sleep || data->is_dark_screen) {
        bool was_sleep = data->is_sleep;
        bool was_dark = data->is_dark_screen;

        data->is_sleep       = false;
        data->is_dark_screen = false;

        DEBUG_LOG(">>> Touch wake-up (previous: dark_screen=%d, sleep=%d)\n", was_dark, was_sleep);
        update_screen_luminance();
    }
    data->lastTouchTime = lv_tick_get();
}

int main(int argc, char * argv[])
{
    for(int i = 1; i < argc; i++) {
        if(strcmp(argv[i], "debug") == 0) {
            g_debug_log = true;
            break;
        }
    }

    lv_init();

    fbdev_init();

    // clean framebuffer
    static lv_color_t buf[DISP_BUF_SIZE];
    static lv_disp_draw_buf_t disp_buf;
    lv_disp_draw_buf_init(&disp_buf, buf, NULL, DISP_BUF_SIZE);

    //  initialize and register the display driver
    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.draw_buf  = &disp_buf;
    disp_drv.flush_cb  = fbdev_flush;
    disp_drv.hor_res   = 240;
    disp_drv.ver_res   = 284;
    disp_drv.sw_rotate = 1;
    disp_drv.rotated   = LV_DISP_ROT_90;
    lv_disp_t * disp   = lv_disp_drv_register(&disp_drv);

    // initialize and register the input device driver
    evdev_init();
    static lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);
    indev_drv.type        = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb     = evdev_read;
    indev_drv.feedback_cb = my_touch_feedback_cb;
    lv_indev_drv_register(&indev_drv);
    lv_obj_clean(lv_scr_act()); // clean the screen
    lv_obj_set_style_bg_color(lv_scr_act(), lv_color_hex(0x000000), LV_PART_MAIN);

    // initialize configuration manager
    config_init();
    
   

    DEBUG_LOG("arkkvm_ui UI initialization started.\n");

    data_manager_init();
    page_manager_init();
    DEBUG_LOG("Page manager initialized.\n");
    ipc_server_start("/tmp/arkkvm_ui.sock");
    // setup data update callback to refresh pages
    data_manager_register_callback(page_update_all);

    // create a periodic timer to update data
    lv_timer_create(timer_callback, 2000, NULL);
    lv_timer_create(screen_timer_callback, 1000, NULL);
    // main loop



    is_init_ui = true;
    while(1) {
        lv_timer_handler(); // let the GUI do its work
        usleep(5000);       // sleep for 5 milliseconds
    }

    return 0;
}

uint32_t custom_tick_get(void)
{
    static uint64_t start_ms = 0;
    if(start_ms == 0) {
        struct timeval tv_start;
        gettimeofday(&tv_start, NULL);
        start_ms = (tv_start.tv_sec * 1000000 + tv_start.tv_usec) / 1000;
    }

    struct timeval tv_now;
    gettimeofday(&tv_now, NULL);
    uint64_t now_ms;
    now_ms = (tv_now.tv_sec * 1000000 + tv_now.tv_usec) / 1000;

    uint32_t time_ms = now_ms - start_ms;
    return time_ms;
}
