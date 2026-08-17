#ifndef UI_JOB_QUEUE_H
#define UI_JOB_QUEUE_H

#include <stddef.h>

/*
 * Config IPC must not call LVGL / popen / config_save from the socket thread.
 * IPC posts coalesced jobs; the LVGL thread drains via ui_job_process_pending().
 * GET_CONFIG reads GlobalData (kept up to date when a job is posted).
 */

void ui_job_post_orientation(int orientation);
void ui_job_post_luminance(int luminance);
void ui_job_post_dark_screen_time(int sec);
void ui_job_post_sleep_time(int sec);

void ui_job_format_config_response(char * buf, size_t buf_size);

/* LVGL / main thread only. No-op while hold/busy or when empty. */
void ui_job_process_pending(void);

#endif
