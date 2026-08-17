#include "view_hold_action.h"
#include "../utils/font_util.h"
#include <stdio.h>
#include <string.h>

/* 100ms is enough for ~2–3px steps and halves fbdev/sw_rotate load vs 50ms. */
#define HOLD_TIMER_PERIOD_MS 100
#define HOLD_BTN_BORDER      2
#define HOLD_COLOR_WARN      0xDC2626

static int g_hold_block_count = 0;

static void apply_idle_visuals(ViewHoldAction * action);
static void stop_timer(ViewHoldAction * action);
static void cancel_hold(ViewHoldAction * action);

static void hold_block_acquire(void)
{
    g_hold_block_count++;
}

static void hold_block_release(void)
{
    if(g_hold_block_count > 0) {
        g_hold_block_count--;
    }
}

bool view_hold_action_blocks_background(void)
{
    return g_hold_block_count > 0;
}

/*
 * Figma fill shares the same outer rect as the pill (including under the 2px
 * stroke). LVGL children sit inside the border, so offset by -border and size
 * to the full button so fill sits flush against the stroke with no gap.
 */
static void set_progress_width(ViewHoldAction * action, uint32_t elapsed_ms)
{
    lv_coord_t w;

    if(action->progress == NULL) {
        return;
    }

    if(elapsed_ms >= HOLD_ACTION_MS) {
        w = HOLD_ACTION_BTN_W;
    } else {
        w = (lv_coord_t)(((uint64_t)elapsed_ms * (uint64_t)HOLD_ACTION_BTN_W) /
                         HOLD_ACTION_MS);
    }
    /* Skip LVGL invalidate when width unchanged (avoids useless full flushes). */
    if(w == action->last_progress_w) {
        return;
    }
    action->last_progress_w = w;
    lv_obj_set_width(action->progress, w);
}

static void show_hold_chrome(ViewHoldAction * action, bool show)
{
    if(show) {
        if(action->hold_line) {
            lv_obj_clear_flag(action->hold_line, LV_OBJ_FLAG_HIDDEN);
        }
        if(action->hold_for_label) {
            lv_obj_clear_flag(action->hold_for_label, LV_OBJ_FLAG_HIDDEN);
        }
        if(action->seconds_label) {
            lv_obj_clear_flag(action->seconds_label, LV_OBJ_FLAG_HIDDEN);
        }
        if(action->countdown_label) {
            lv_obj_clear_flag(action->countdown_label, LV_OBJ_FLAG_HIDDEN);
        }
    } else if(action->hold_line) {
        lv_obj_add_flag(action->hold_line, LV_OBJ_FLAG_HIDDEN);
    } else {
        if(action->hold_for_label) {
            lv_obj_add_flag(action->hold_for_label, LV_OBJ_FLAG_HIDDEN);
        }
        if(action->seconds_label) {
            lv_obj_add_flag(action->seconds_label, LV_OBJ_FLAG_HIDDEN);
        }
        if(action->countdown_label) {
            lv_obj_add_flag(action->countdown_label, LV_OBJ_FLAG_HIDDEN);
        }
    }
}

static void apply_idle_visuals(ViewHoldAction * action)
{
    show_hold_chrome(action, true);

    if(action->countdown_label) {
        lv_label_set_text(action->countdown_label, "5");
    }
    if(action->action_label) {
        lv_label_set_text(action->action_label, action->action_default);
        lv_obj_set_style_text_font(action->action_label, font_sls(FS_FONT_REGULAR_20),
                                   LV_PART_MAIN);
        lv_obj_set_width(action->action_label, LV_SIZE_CONTENT);
        lv_obj_clear_flag(action->action_label, LV_OBJ_FLAG_HIDDEN);
        lv_obj_align(action->action_label, LV_ALIGN_TOP_MID, 0, HOLD_ACTION_LINE_Y);
    }
    action->last_progress_w = -1;
    set_progress_width(action, 0);
    if(action->btn) {
        lv_obj_clear_flag(action->btn, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(action->btn, LV_OBJ_FLAG_CLICKABLE);
    }
    if(action->btn_label) {
        lv_label_set_text(action->btn_label, action->btn_default);
        lv_obj_move_foreground(action->btn_label);
    }
    action->last_second = 5;
}

static void stop_timer(ViewHoldAction * action)
{
    if(action->timer) {
        lv_timer_pause(action->timer);
    }
}

/* Run on_done on a later timer tick so busy/"Restoring..." can paint first. */
static void deferred_done_cb(lv_timer_t * timer)
{
    ViewHoldAction * action = (ViewHoldAction *)timer->user_data;

    if(action) {
        action->done_timer = NULL;
        if(action->on_done) {
            action->on_done(action, action->user_data);
        }
    }
    /* Timer has repeat_count=1 and is deleted by LVGL after this callback. */
}

static void finish_hold(ViewHoldAction * action)
{
    if(action->completed) {
        return;
    }

    action->completed = true;
    if(action->is_holding) {
        hold_block_release();
    }
    action->is_holding = false;
    stop_timer(action);

    /* Full fill first, then switch to Figma busy page (no button). */
    set_progress_width(action, HOLD_ACTION_MS);
    if(action->busy_default[0] != '\0') {
        /* set_busy acquires block while busy text is shown */
        view_hold_action_set_busy(action, action->busy_default);
    } else {
        show_hold_chrome(action, false);
    }

    if(action->on_done == NULL) {
        return;
    }

    if(action->done_timer) {
        lv_timer_del(action->done_timer);
        action->done_timer = NULL;
    }
    /* One-shot: LVGL deletes the timer after the callback returns. */
    action->done_timer = lv_timer_create(deferred_done_cb, 30, action);
    lv_timer_set_repeat_count(action->done_timer, 1);
}

static bool action_page_hidden(const ViewHoldAction * action)
{
    lv_obj_t * obj = action->btn;
    while(obj) {
        if(lv_obj_has_flag(obj, LV_OBJ_FLAG_HIDDEN)) {
            return true;
        }
        obj = lv_obj_get_parent(obj);
    }
    return false;
}

static void hold_timer_cb(lv_timer_t * timer)
{
    ViewHoldAction * action = (ViewHoldAction *)timer->user_data;
    if(action == NULL || !action->is_holding || action->completed || action->is_busy) {
        return;
    }

    if(action_page_hidden(action)) {
        cancel_hold(action);
        return;
    }

    uint32_t elapsed = lv_tick_elaps(action->hold_start_tick);
    if(elapsed > HOLD_ACTION_MS) {
        elapsed = HOLD_ACTION_MS;
    }

    set_progress_width(action, elapsed);

    if(elapsed >= HOLD_ACTION_MS) {
        finish_hold(action);
        return;
    }

    int sec = HOLD_ACTION_SECONDS - (int)(elapsed / 1000U);
    if(sec < 1) {
        sec = 1;
    }
    if(sec != action->last_second && action->countdown_label) {
        char buf[8];
        action->last_second = sec;
        lv_snprintf(buf, sizeof(buf), "%d", sec);
        lv_label_set_text(action->countdown_label, buf);
    }
}

static void start_hold(ViewHoldAction * action)
{
    if(action->is_holding || action->completed || action->is_busy) {
        return;
    }

    action->is_holding = true;
    hold_block_acquire();
    action->hold_start_tick = lv_tick_get();
    action->last_second     = -1;
    action->last_progress_w = -1;

    set_progress_width(action, 0);
    if(action->countdown_label) {
        lv_label_set_text(action->countdown_label, "5");
        lv_obj_clear_flag(action->countdown_label, LV_OBJ_FLAG_HIDDEN);
        action->last_second = 5;
    }
    if(action->hold_line) {
        lv_obj_clear_flag(action->hold_line, LV_OBJ_FLAG_HIDDEN);
    }

    if(action->timer == NULL) {
        action->timer = lv_timer_create(hold_timer_cb, HOLD_TIMER_PERIOD_MS, action);
    } else {
        lv_timer_resume(action->timer);
        lv_timer_reset(action->timer);
    }
}

static void cancel_hold(ViewHoldAction * action)
{
    if(action->completed || action->is_busy) {
        return;
    }

    if(action->is_holding) {
        hold_block_release();
    }
    action->is_holding = false;
    stop_timer(action);
    apply_idle_visuals(action);
}

static void btn_event_cb(lv_event_t * e)
{
    ViewHoldAction * action = (ViewHoldAction *)lv_event_get_user_data(e);
    lv_event_code_t code = lv_event_get_code(e);

    if(action == NULL) {
        return;
    }

    if(code == LV_EVENT_PRESSED) {
        start_hold(action);
    } else if(code == LV_EVENT_RELEASED || code == LV_EVENT_PRESS_LOST) {
        cancel_hold(action);
    }
}

void view_hold_action_init(ViewHoldAction * action, lv_obj_t * parent,
                           const ViewHoldActionConfig * cfg)
{
    memset(action, 0, sizeof(*action));
    action->on_done = cfg->on_done;
    action->user_data = cfg->user_data;
    action->last_second     = -1;
    action->last_progress_w = -1;

    strncpy(action->action_default, cfg->action_text ? cfg->action_text : "",
            sizeof(action->action_default) - 1);
    strncpy(action->btn_default, cfg->btn_text ? cfg->btn_text : "",
            sizeof(action->btn_default) - 1);
    strncpy(action->busy_default, cfg->busy_text ? cfg->busy_text : "",
            sizeof(action->busy_default) - 1);

    /* "Hold for N seconds": bottom-aligned row, equal gap around the digit. */
    action->hold_line = lv_obj_create(parent);
    lv_obj_set_size(action->hold_line, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_set_style_bg_opa(action->hold_line, LV_OPA_TRANSP, LV_PART_MAIN);
    lv_obj_set_style_border_width(action->hold_line, 0, LV_PART_MAIN);
    lv_obj_set_style_radius(action->hold_line, 0, LV_PART_MAIN);
    lv_obj_set_style_pad_all(action->hold_line, 0, LV_PART_MAIN);
    lv_obj_set_style_pad_column(action->hold_line, HOLD_ACTION_DIGIT_GAP, LV_PART_MAIN);
    lv_obj_clear_flag(action->hold_line, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_clear_flag(action->hold_line, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_set_flex_flow(action->hold_line, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(action->hold_line, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_END,
                          LV_FLEX_ALIGN_CENTER);
    lv_obj_align(action->hold_line, LV_ALIGN_TOP_MID, 0, HOLD_ACTION_HOLD_LINE_Y);

    action->hold_for_label = lv_label_create(action->hold_line);
    lv_label_set_text(action->hold_for_label, "Hold for");
    lv_obj_set_style_text_color(action->hold_for_label, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    lv_obj_set_style_text_font(action->hold_for_label, font_sls(FS_FONT_REGULAR_20), LV_PART_MAIN);

    action->countdown_label = lv_label_create(action->hold_line);
    lv_label_set_text(action->countdown_label, "5");
    lv_obj_set_style_text_color(action->countdown_label, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    lv_obj_set_style_text_font(action->countdown_label, font_sls(FS_FONT_REGULAR_30), LV_PART_MAIN);
    /* Sit 2px below the side-text bottom edge. */
    lv_obj_set_style_translate_y(action->countdown_label, 2, LV_PART_MAIN);

    action->seconds_label = lv_label_create(action->hold_line);
    lv_label_set_text(action->seconds_label, "seconds");
    lv_obj_set_style_text_color(action->seconds_label, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    lv_obj_set_style_text_font(action->seconds_label, font_sls(FS_FONT_REGULAR_20), LV_PART_MAIN);

    action->action_label = lv_label_create(parent);
    lv_label_set_text(action->action_label, action->action_default);
    lv_obj_set_style_text_color(action->action_label, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    lv_obj_set_style_text_font(action->action_label, font_sls(FS_FONT_REGULAR_20), LV_PART_MAIN);
    lv_obj_set_style_text_align(action->action_label, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_align(action->action_label, LV_ALIGN_TOP_MID, 0, HOLD_ACTION_LINE_Y);

    /* Pill track: 20% red + 2px stroke (Figma Frame 26). */
    action->btn = lv_obj_create(parent);
    lv_obj_set_size(action->btn, HOLD_ACTION_BTN_W, HOLD_ACTION_BTN_H);
    lv_obj_set_pos(action->btn, HOLD_ACTION_BTN_X, HOLD_ACTION_BTN_Y);
    lv_obj_set_style_radius(action->btn, LV_RADIUS_CIRCLE, LV_PART_MAIN);
    lv_obj_set_style_clip_corner(action->btn, true, LV_PART_MAIN);
    lv_obj_set_style_bg_color(action->btn, lv_color_hex(HOLD_COLOR_WARN), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(action->btn, LV_OPA_20, LV_PART_MAIN);
    lv_obj_set_style_border_color(action->btn, lv_color_hex(HOLD_COLOR_WARN), LV_PART_MAIN);
    lv_obj_set_style_border_width(action->btn, HOLD_BTN_BORDER, LV_PART_MAIN);
    lv_obj_set_style_border_opa(action->btn, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_set_style_pad_all(action->btn, 0, LV_PART_MAIN);
    lv_obj_clear_flag(action->btn, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_clear_flag(action->btn, LV_OBJ_FLAG_OVERFLOW_VISIBLE);
    lv_obj_set_scrollbar_mode(action->btn, LV_SCROLLBAR_MODE_OFF);
    lv_obj_add_flag(action->btn, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_event_cb(action->btn, btn_event_cb, LV_EVENT_ALL, action);

    /*
     * Progress fill (Figma Frame 27): same outer size as the pill, grows in
     * width. Offset by -border so it sits flush under/against the stroke.
     */
    action->progress = lv_obj_create(action->btn);
    lv_obj_set_size(action->progress, 0, HOLD_ACTION_BTN_H);
    lv_obj_set_pos(action->progress, -HOLD_BTN_BORDER, -HOLD_BTN_BORDER);
    lv_obj_set_style_radius(action->progress, LV_RADIUS_CIRCLE, LV_PART_MAIN);
    lv_obj_set_style_bg_color(action->progress, lv_color_hex(HOLD_COLOR_WARN), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(action->progress, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_set_style_border_width(action->progress, 0, LV_PART_MAIN);
    lv_obj_set_style_pad_all(action->progress, 0, LV_PART_MAIN);
    lv_obj_clear_flag(action->progress, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_clear_flag(action->progress, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_add_flag(action->progress, LV_OBJ_FLAG_EVENT_BUBBLE);

    action->btn_label = lv_label_create(action->btn);
    lv_label_set_text(action->btn_label, action->btn_default);
    lv_obj_set_style_text_color(action->btn_label, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    lv_obj_set_style_text_font(action->btn_label, font_sls(FS_FONT_REGULAR_20), LV_PART_MAIN);
    lv_obj_align(action->btn_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_add_flag(action->btn_label, LV_OBJ_FLAG_EVENT_BUBBLE);
    lv_obj_move_foreground(action->btn_label);

    action->last_second = 5;
}

void view_hold_action_reset_idle(ViewHoldAction * action)
{
    if(action == NULL) {
        return;
    }
    if(action->done_timer) {
        lv_timer_del(action->done_timer);
        action->done_timer = NULL;
    }
    if(action->is_holding || action->is_busy) {
        hold_block_release();
    }
    action->is_holding = false;
    action->is_busy = false;
    action->completed = false;
    stop_timer(action);
    apply_idle_visuals(action);
}

void view_hold_action_set_busy(ViewHoldAction * action, const char * message)
{
    if(action == NULL) {
        return;
    }

    /* Figma busy — hide pill + hold chrome; centered status only (Regular 24). */
    if(!action->is_busy) {
        hold_block_acquire();
    }
    action->is_busy = true;
    if(action->is_holding) {
        hold_block_release();
    }
    action->is_holding = false;
    stop_timer(action);

    if(action->btn) {
        lv_obj_add_flag(action->btn, LV_OBJ_FLAG_HIDDEN);
    }
    show_hold_chrome(action, false);
    if(action->action_label && message) {
        lv_label_set_text(action->action_label, message);
        lv_obj_set_style_text_font(action->action_label, font_sls(FS_FONT_REGULAR_24),
                                   LV_PART_MAIN);
        lv_obj_set_width(action->action_label, LV_SIZE_CONTENT);
        lv_obj_clear_flag(action->action_label, LV_OBJ_FLAG_HIDDEN);
        lv_obj_align(action->action_label, LV_ALIGN_TOP_MID, 0, HOLD_ACTION_BUSY_Y);
    }
}

void view_hold_action_cleanup(ViewHoldAction * action)
{
    if(action == NULL) {
        return;
    }
    if(action->is_holding || action->is_busy) {
        hold_block_release();
    }
    if(action->timer) {
        lv_timer_del(action->timer);
        action->timer = NULL;
    }
    if(action->done_timer) {
        lv_timer_del(action->done_timer);
        action->done_timer = NULL;
    }
    memset(action, 0, sizeof(*action));
}
