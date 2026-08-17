#ifndef VIEW_HOLD_ACTION_H
#define VIEW_HOLD_ACTION_H

#include "../../lvgl/lvgl.h"
#include <stdbool.h>

#define HOLD_ACTION_MS           5000
#define HOLD_ACTION_SECONDS      5
#define HOLD_ACTION_BTN_X        16
#define HOLD_ACTION_BTN_Y        160
#define HOLD_ACTION_BTN_W        252
#define HOLD_ACTION_BTN_H        64
/*
 * Hold line ("Hold for" + digit + "seconds"): flex row, cross-align bottom.
 * HOLD_ACTION_HOLD_LINE_Y is the top of the taller digit (R30 h=28); side
 * labels (R20 h=19) sit on the same bottom edge → their top lands at 90.
 */
#define HOLD_ACTION_HOLD_LINE_Y  81
#define HOLD_ACTION_DIGIT_GAP    9
#define HOLD_ACTION_LINE_Y       120
#define HOLD_ACTION_BUSY_Y       137

typedef struct ViewHoldAction ViewHoldAction;

typedef void (*view_hold_action_done_cb_t)(ViewHoldAction * action, void * user_data);

typedef struct {
    /* Bottom line under "Hold for N seconds" (e.g. "to Reboot"). */
    const char * action_text;
    const char * btn_text;
    /* Shown when hold completes (e.g. "Restoring..." / "Rebooting...") */
    const char * busy_text;
    view_hold_action_done_cb_t on_done;
    void * user_data;
} ViewHoldActionConfig;

struct ViewHoldAction {
    lv_obj_t * hold_line;
    lv_obj_t * hold_for_label;
    lv_obj_t * seconds_label;
    lv_obj_t * action_label;
    lv_obj_t * countdown_label;
    lv_obj_t * btn;
    lv_obj_t * progress;
    lv_obj_t * btn_label;
    lv_timer_t * timer;
    lv_timer_t * done_timer;
    view_hold_action_done_cb_t on_done;
    void * user_data;
    char action_default[96];
    char btn_default[48];
    char busy_default[48];
    uint32_t hold_start_tick;
    int last_second;
    lv_coord_t last_progress_w;
    bool is_holding;
    bool is_busy;
    bool completed;
};

void view_hold_action_init(ViewHoldAction * action, lv_obj_t * parent,
                           const ViewHoldActionConfig * cfg);
void view_hold_action_reset_idle(ViewHoldAction * action);
void view_hold_action_set_busy(ViewHoldAction * action, const char * message);
void view_hold_action_cleanup(ViewHoldAction * action);

/* True while any hold/busy gesture is active — pause all UI-thread polling. */
bool view_hold_action_blocks_background(void);

#endif
