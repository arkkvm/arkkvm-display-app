#include "settings_factory_reset_page.h"
#include "../components/view_navigation.h"
#include "../core/page_manager.h"
#include "../core/socket_server.h"
#include "../utils/debug.h"
#include "../utils/font_util.h"
#include "../utils/view_util.h"
#include <pthread.h>
#include <string.h>

/* Wipe + reboot spawn; far shorter than network refresh. */
#define FACTORY_RESET_IPC_TIMEOUT_SEC 30
#define FACTORY_RESET_POLL_MS         50

/* Figma 975:142 confirm stage (coords relative to content below NAV_HEIGHT) */
#define FACTORY_WARN_X        24
#define FACTORY_WARN_Y        (79 - NAV_HEIGHT)   /* absolute 79 → 15 */
#define FACTORY_WARN_W        236
/* Figma 975:152 lineHeight 140% × 16 ≈ 22; font line_height=15 → extra 7 */
#define FACTORY_WARN_LINE_SPACE 7
#define FACTORY_CONTINUE_X    16
#define FACTORY_CONTINUE_Y    (160 - NAV_HEIGHT)  /* absolute 160 → 96 */
#define FACTORY_CONTINUE_W    252
#define FACTORY_CONTINUE_H    64
#define FACTORY_CONTINUE_BORDER 2
#define FACTORY_COLOR_ACCENT  0x60A5FA

static SettingsFactoryResetPageUI ui;
static lv_obj_t * current_parent = NULL;
static lv_timer_t * reset_result_timer = NULL;
static bool s_show_confirm_on_enter = false;
static bool s_on_confirm_stage = true;

/* Written by IPC worker; read by LVGL timer only. */
static volatile int reset_ipc_pending = 0;
static volatile int reset_ipc_done    = 0;
static volatile int reset_ipc_ok     = 0;

static void factory_reset_show_confirm(void);
static void factory_reset_show_hold(void);

void settings_factory_reset_page_request_confirm(void)
{
    s_show_confirm_on_enter = true;
}

static void factory_reset_show_failed(void)
{
    if(ui.hold.hold_line) {
        lv_obj_add_flag(ui.hold.hold_line, LV_OBJ_FLAG_HIDDEN);
    }
    if(ui.hold.hold_for_label) {
        lv_obj_add_flag(ui.hold.hold_for_label, LV_OBJ_FLAG_HIDDEN);
    }
    if(ui.hold.seconds_label) {
        lv_obj_add_flag(ui.hold.seconds_label, LV_OBJ_FLAG_HIDDEN);
    }
    if(ui.hold.countdown_label) {
        lv_obj_add_flag(ui.hold.countdown_label, LV_OBJ_FLAG_HIDDEN);
    }
    if(ui.hold.action_label) {
        lv_label_set_text(ui.hold.action_label, "Reset failed");
        lv_obj_set_style_text_font(ui.hold.action_label, font_sls(FS_FONT_REGULAR_24),
                                   LV_PART_MAIN);
        lv_obj_align(ui.hold.action_label, LV_ALIGN_TOP_MID, 0, HOLD_ACTION_BUSY_Y);
    }
}

static void factory_reset_stop_result_timer(void)
{
    if(reset_result_timer) {
        lv_timer_del(reset_result_timer);
        reset_result_timer = NULL;
    }
}

static void factory_reset_result_timer_cb(lv_timer_t * timer)
{
    int ok;
    lv_timer_t * self = timer;

    if(!reset_ipc_done) {
        return;
    }

    ok = reset_ipc_ok;
    reset_ipc_done    = 0;
    reset_ipc_pending = 0;
    reset_result_timer = NULL;
    lv_timer_set_repeat_count(self, 0);

    DEBUG_LOG("[factoryreset] done ok=%d current=%d\n", ok, (int)page_get_current());

    if(current_parent == NULL) {
        return;
    }

    if(ok) {
        /* Device is rebooting; keep "Resetting..." visible. */
        return;
    }

    view_hold_action_reset_idle(&ui.hold);
    if(page_get_current() == PAGE_SETTINGS_FACTORY_RESET && !s_on_confirm_stage) {
        factory_reset_show_failed();
    }
}

static void * factory_reset_ipc_thread(void * arg)
{
    char resp[256];
    bool ok;

    LV_UNUSED(arg);
    ok = ipc_send_command_timeout("FACTORY_RESET", resp, sizeof(resp),
                                  FACTORY_RESET_IPC_TIMEOUT_SEC);
    DEBUG_LOG("[factoryreset] ipc ok=%d resp='%s'\n", ok ? 1 : 0, resp);
    reset_ipc_ok   = ok ? 1 : 0;
    reset_ipc_done = 1;
    return NULL;
}

static void factory_reset_hold_done(ViewHoldAction * action, void * user_data)
{
    pthread_t tid;

    LV_UNUSED(action);
    LV_UNUSED(user_data);

    if(reset_ipc_pending) {
        return;
    }

    reset_ipc_pending = 1;
    reset_ipc_done    = 0;
    reset_ipc_ok      = 0;

    DEBUG_LOG("[factoryreset] start current=%d\n", (int)page_get_current());

    factory_reset_stop_result_timer();
    reset_result_timer =
        lv_timer_create(factory_reset_result_timer_cb, FACTORY_RESET_POLL_MS, NULL);

    if(pthread_create(&tid, NULL, factory_reset_ipc_thread, NULL) != 0) {
        reset_ipc_pending = 0;
        factory_reset_stop_result_timer();
        view_hold_action_reset_idle(&ui.hold);
        factory_reset_show_failed();
        return;
    }
    pthread_detach(tid);
}

static void factory_reset_hide_hold_widgets(void)
{
    if(ui.hold.hold_line) {
        lv_obj_add_flag(ui.hold.hold_line, LV_OBJ_FLAG_HIDDEN);
    }
    if(ui.hold.hold_for_label) {
        lv_obj_add_flag(ui.hold.hold_for_label, LV_OBJ_FLAG_HIDDEN);
    }
    if(ui.hold.seconds_label) {
        lv_obj_add_flag(ui.hold.seconds_label, LV_OBJ_FLAG_HIDDEN);
    }
    if(ui.hold.countdown_label) {
        lv_obj_add_flag(ui.hold.countdown_label, LV_OBJ_FLAG_HIDDEN);
    }
    if(ui.hold.action_label) {
        lv_obj_add_flag(ui.hold.action_label, LV_OBJ_FLAG_HIDDEN);
    }
    if(ui.hold.btn) {
        lv_obj_add_flag(ui.hold.btn, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(ui.hold.btn, LV_OBJ_FLAG_CLICKABLE);
    }
}

static void factory_reset_show_confirm(void)
{
    s_on_confirm_stage = true;
    if(ui.hold.is_holding || ui.hold.is_busy) {
        view_hold_action_reset_idle(&ui.hold);
    } else if(!ui.hold.completed) {
        view_hold_action_reset_idle(&ui.hold);
    }
    factory_reset_hide_hold_widgets();
    if(ui.confirm_panel) {
        lv_obj_clear_flag(ui.confirm_panel, LV_OBJ_FLAG_HIDDEN);
    }
    view_navigation_raise();
}

static void factory_reset_show_hold(void)
{
    s_on_confirm_stage = false;
    if(ui.confirm_panel) {
        lv_obj_add_flag(ui.confirm_panel, LV_OBJ_FLAG_HIDDEN);
    }
    view_hold_action_reset_idle(&ui.hold);
    view_navigation_raise();
}

static void continue_btn_event_cb(lv_event_t * e)
{
    if(lv_event_get_code(e) != LV_EVENT_CLICKED) {
        return;
    }
    factory_reset_show_hold();
}

void settings_factory_reset_page_init(lv_obj_t * parent)
{
    ViewHoldActionConfig cfg;

    current_parent = parent;
    reset_ipc_pending = 0;
    reset_ipc_done    = 0;
    reset_ipc_ok      = 0;
    factory_reset_stop_result_timer();

    ui.container = are_create_view(parent);
    lv_obj_set_size(ui.container, LV_PCT(100), LV_PCT(100));
    lv_obj_set_style_bg_color(ui.container, lv_color_hex(0x000000), LV_PART_MAIN);
    lv_obj_clear_flag(ui.container, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_scrollbar_mode(ui.container, LV_SCROLLBAR_MODE_OFF);

    view_navigation_init(ui.container, "Settings", "Factory Reset");

    /* Hold stage (hidden until Continue). */
    memset(&cfg, 0, sizeof(cfg));
    cfg.action_text = "Reset ArkKVM";
    cfg.btn_text = "Factory Reset";
    cfg.busy_text = "Resetting...";
    cfg.on_done = factory_reset_hold_done;
    cfg.user_data = NULL;
    view_hold_action_init(&ui.hold, ui.container, &cfg);

    /* Confirm stage — Figma 975:142 (content area only; do not cover nav). */
    ui.confirm_panel = are_create_view(ui.container);
    lv_obj_set_pos(ui.confirm_panel, 0, NAV_HEIGHT);
    lv_obj_set_size(ui.confirm_panel, LV_PCT(100), 240 - NAV_HEIGHT);
    lv_obj_set_style_bg_opa(ui.confirm_panel, LV_OPA_TRANSP, LV_PART_MAIN);
    lv_obj_clear_flag(ui.confirm_panel, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_clear_flag(ui.confirm_panel, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_set_scrollbar_mode(ui.confirm_panel, LV_SCROLLBAR_MODE_OFF);

    ui.warn_label = lv_label_create(ui.confirm_panel);
    lv_label_set_text(ui.warn_label,
        "Factory Reset will permanently erase all local data and restore ArkKVM to its factory settings.");
    lv_obj_set_style_text_color(ui.warn_label, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    lv_obj_set_style_text_font(ui.warn_label, font_sls(FS_FONT_REGULAR_16), LV_PART_MAIN);
    /* Match Figma 140% line height (box h=66 for ~3 lines). */
    lv_obj_set_style_text_line_space(ui.warn_label, FACTORY_WARN_LINE_SPACE, LV_PART_MAIN);
    lv_obj_set_style_text_align(ui.warn_label, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);
    lv_obj_set_width(ui.warn_label, FACTORY_WARN_W);
    lv_label_set_long_mode(ui.warn_label, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui.warn_label, FACTORY_WARN_X, FACTORY_WARN_Y);

    ui.continue_btn = lv_obj_create(ui.confirm_panel);
    lv_obj_set_size(ui.continue_btn, FACTORY_CONTINUE_W, FACTORY_CONTINUE_H);
    lv_obj_set_pos(ui.continue_btn, FACTORY_CONTINUE_X, FACTORY_CONTINUE_Y);
    lv_obj_set_style_radius(ui.continue_btn, LV_RADIUS_CIRCLE, LV_PART_MAIN);
    lv_obj_set_style_bg_color(ui.continue_btn, lv_color_hex(FACTORY_COLOR_ACCENT), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(ui.continue_btn, LV_OPA_20, LV_PART_MAIN);
    lv_obj_set_style_border_color(ui.continue_btn, lv_color_hex(FACTORY_COLOR_ACCENT), LV_PART_MAIN);
    lv_obj_set_style_border_width(ui.continue_btn, FACTORY_CONTINUE_BORDER, LV_PART_MAIN);
    lv_obj_set_style_border_opa(ui.continue_btn, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_set_style_pad_all(ui.continue_btn, 0, LV_PART_MAIN);
    lv_obj_clear_flag(ui.continue_btn, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_scrollbar_mode(ui.continue_btn, LV_SCROLLBAR_MODE_OFF);
    lv_obj_add_flag(ui.continue_btn, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_event_cb(ui.continue_btn, continue_btn_event_cb, LV_EVENT_CLICKED, NULL);

    ui.continue_label = lv_label_create(ui.continue_btn);
    lv_label_set_text(ui.continue_label, "Continue");
    lv_obj_set_style_text_color(ui.continue_label, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    lv_obj_set_style_text_font(ui.continue_label, font_sls(FS_FONT_REGULAR_20), LV_PART_MAIN);
    lv_obj_align(ui.continue_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_add_flag(ui.continue_label, LV_OBJ_FLAG_EVENT_BUBBLE);

    view_navigation_raise();
    factory_reset_show_confirm();
}

void settings_factory_reset_page_update(void)
{
    /*
     * Only snap to confirm on navigation enter — not on page_update_all /
     * data refresh (same pattern as Settings menu scroll-to-top).
     */
    if(s_show_confirm_on_enter) {
        s_show_confirm_on_enter = false;
        /* Keep busy/"Resetting..." if wipe already started. */
        if(!reset_ipc_pending && !ui.hold.is_busy) {
            factory_reset_show_confirm();
        }
        return;
    }

    if(s_on_confirm_stage) {
        return;
    }

    if(!ui.hold.completed && !ui.hold.is_busy && !ui.hold.is_holding) {
        view_hold_action_reset_idle(&ui.hold);
    }
}

void settings_factory_reset_page_cleanup(void)
{
    if(current_parent) {
        factory_reset_stop_result_timer();
        view_hold_action_cleanup(&ui.hold);
        lv_obj_del(ui.container);
        current_parent = NULL;
        s_on_confirm_stage = true;
        s_show_confirm_on_enter = false;
    }
}
