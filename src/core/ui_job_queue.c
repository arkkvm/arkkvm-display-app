#include "ui_job_queue.h"

#include "../../lvgl/lvgl.h"
#include "../components/view_hold_action.h"
#include "../utils/command.h"
#include "../utils/debug.h"
#include "config_manager.h"
#include "data_manager.h"
#include "page_manager.h"

#include <pthread.h>
#include <stdio.h>
#include <string.h>

typedef enum {
    UI_JOB_ORIENT = 0,
    UI_JOB_LUM,
    UI_JOB_DARK,
    UI_JOB_SLEEP,
    UI_JOB_TYPE_COUNT
} UiJobType;

static pthread_mutex_t g_job_mutex = PTHREAD_MUTEX_INITIALIZER;
static int g_job_pending[UI_JOB_TYPE_COUNT];
static int g_job_value[UI_JOB_TYPE_COUNT];

static void ui_job_enqueue(UiJobType type, int value)
{
    if(type < 0 || type >= UI_JOB_TYPE_COUNT) {
        return;
    }

    pthread_mutex_lock(&g_job_mutex);
    /* Coalesce: same type keeps only the latest value. */
    g_job_pending[type] = 1;
    g_job_value[type]   = value;
    pthread_mutex_unlock(&g_job_mutex);
}

void ui_job_post_orientation(int orientation)
{
    GlobalData * data = page_get_global_data();

    orientation = (orientation == 1) ? 1 : 0;
    /* Snapshot for GET_CONFIG; LVGL apply happens on the UI thread. */
    data->orientation = orientation;
    ui_job_enqueue(UI_JOB_ORIENT, orientation);
}

void ui_job_post_luminance(int luminance)
{
    GlobalData * data = page_get_global_data();

    data->screen_luminance = luminance;
    ui_job_enqueue(UI_JOB_LUM, luminance);
}

void ui_job_post_dark_screen_time(int sec)
{
    GlobalData * data = page_get_global_data();

    data->dark_screen_time = sec;
    ui_job_enqueue(UI_JOB_DARK, sec);
}

void ui_job_post_sleep_time(int sec)
{
    GlobalData * data = page_get_global_data();

    data->sleep_time = sec;
    ui_job_enqueue(UI_JOB_SLEEP, sec);
}

void ui_job_format_config_response(char * buf, size_t buf_size)
{
    GlobalData * data;

    if(buf == NULL || buf_size == 0) {
        return;
    }

    data = page_get_global_data();
    snprintf(buf, buf_size,
             "ORIENTATION=%d,LUMINANCE=%d,DARK_SCREEN_TIME=%d,SLEEP_TIME=%d",
             data->orientation, data->screen_luminance, data->dark_screen_time,
             data->sleep_time);
}

void ui_job_process_pending(void)
{
    int pending[UI_JOB_TYPE_COUNT];
    int value[UI_JOB_TYPE_COUNT];
    int i;
    int any = 0;
    int need_persist = 0;
    GlobalData * data;

    if(view_hold_action_blocks_background()) {
        return;
    }

    pthread_mutex_lock(&g_job_mutex);
    for(i = 0; i < UI_JOB_TYPE_COUNT; i++) {
        pending[i] = g_job_pending[i];
        value[i]   = g_job_value[i];
        if(pending[i]) {
            any = 1;
            g_job_pending[i] = 0;
        }
    }
    pthread_mutex_unlock(&g_job_mutex);

    if(!any) {
        return;
    }

    data = page_get_global_data();

    if(pending[UI_JOB_ORIENT]) {
        data->orientation = value[UI_JOB_ORIENT];
        lv_disp_set_rotation(lv_disp_get_default(),
                             data->orientation == 1 ? LV_DISP_ROT_90 : LV_DISP_ROT_270);
        need_persist = 1;
        DEBUG_LOG("[ui_job] apply orientation=%d\n", data->orientation);
    }

    if(pending[UI_JOB_LUM]) {
        data->screen_luminance = value[UI_JOB_LUM];
        data->lastTouchTime    = lv_tick_get();
        update_screen_luminance();
        need_persist = 1;
        DEBUG_LOG("[ui_job] apply luminance=%d\n", data->screen_luminance);
    }

    if(pending[UI_JOB_DARK]) {
        data->dark_screen_time = value[UI_JOB_DARK];
        need_persist           = 1;
        DEBUG_LOG("[ui_job] apply dark_screen_time=%d\n", data->dark_screen_time);
    }

    if(pending[UI_JOB_SLEEP]) {
        data->sleep_time = value[UI_JOB_SLEEP];
        need_persist     = 1;
        DEBUG_LOG("[ui_job] apply sleep_time=%d\n", data->sleep_time);
    }

    if(need_persist) {
        if(config_update_from_global(data) > 0) {
            data_manager_reset_poll_timer();
        }
    }
}
