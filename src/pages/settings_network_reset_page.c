#include "settings_network_reset_page.h"
#include "../components/view_navigation.h"
#include "../core/page_manager.h"
#include "../core/socket_server.h"
#include "../utils/debug.h"
#include "../utils/font_util.h"
#include "../utils/view_util.h"
#include <pthread.h>
#include <string.h>

/* Factory reset runs refresh-network; allow enough time for DHCP/VLAN cleanup. */
#define NETWORK_RESET_IPC_TIMEOUT_SEC 120
#define NETWORK_RESET_POLL_MS         50

static SettingsNetworkResetPageUI ui;
static lv_obj_t * current_parent = NULL;
static lv_timer_t * reset_result_timer = NULL;

/* Written by IPC worker; read by LVGL timer only. */
static volatile int reset_ipc_pending = 0;
static volatile int reset_ipc_done    = 0;
static volatile int reset_ipc_ok     = 0;

static void network_reset_show_failed(void)
{
    /* Caller already returned hold to idle when needed. */
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
        lv_label_set_text(ui.hold.action_label, "Restore failed");
        lv_obj_set_style_text_font(ui.hold.action_label, font_sls(FS_FONT_REGULAR_24),
                                   LV_PART_MAIN);
        lv_obj_align(ui.hold.action_label, LV_ALIGN_TOP_MID, 0, HOLD_ACTION_BUSY_Y);
    }
}

static void network_reset_stop_result_timer(void)
{
    if(reset_result_timer) {
        lv_timer_del(reset_result_timer);
        reset_result_timer = NULL;
    }
}

static void network_reset_result_timer_cb(lv_timer_t * timer)
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
    /* LVGL deletes this timer after the callback returns. */
    lv_timer_set_repeat_count(self, 0);

    DEBUG_LOG("[netreset] done ok=%d current=%d\n", ok, (int)page_get_current());

    if(current_parent == NULL) {
        return;
    }

    view_hold_action_reset_idle(&ui.hold);

    if(!ok) {
        /*
         * Only paint failure if this page is still the active one.
         * Do not steal focus if the user already navigated elsewhere.
         */
        if(page_get_current() == PAGE_SETTINGS_NETWORK_RESET) {
            network_reset_show_failed();
        }
        return;
    }

    /*
     * Success → always land on home via page_manager.
     * Hides the whole settings stack and shows Main; subsequent Settings
     * entry still works (pages stay is_created, just hidden).
     */
    PAGE_GO_HOME();
    DEBUG_LOG("[netreset] page_go_home -> current=%d\n", (int)page_get_current());
}

static void * network_reset_ipc_thread(void * arg)
{
    char resp[256];
    bool ok;

    LV_UNUSED(arg);
    ok = ipc_send_command_timeout("RESET_NETWORK_FACTORY", resp, sizeof(resp),
                                  NETWORK_RESET_IPC_TIMEOUT_SEC);
    DEBUG_LOG("[netreset] ipc ok=%d resp='%s'\n", ok ? 1 : 0, resp);
    reset_ipc_ok   = ok ? 1 : 0;
    reset_ipc_done = 1;
    return NULL;
}

static void network_reset_hold_done(ViewHoldAction * action, void * user_data)
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

    DEBUG_LOG("[netreset] start current=%d\n", (int)page_get_current());

    network_reset_stop_result_timer();
    reset_result_timer =
        lv_timer_create(network_reset_result_timer_cb, NETWORK_RESET_POLL_MS, NULL);

    if(pthread_create(&tid, NULL, network_reset_ipc_thread, NULL) != 0) {
        reset_ipc_pending = 0;
        network_reset_stop_result_timer();
        view_hold_action_reset_idle(&ui.hold);
        network_reset_show_failed();
        return;
    }
    pthread_detach(tid);
}

void settings_network_reset_page_init(lv_obj_t * parent)
{
    ViewHoldActionConfig cfg;

    current_parent = parent;
    reset_ipc_pending = 0;
    reset_ipc_done    = 0;
    reset_ipc_ok      = 0;
    network_reset_stop_result_timer();

    ui.container = are_create_view(parent);
    lv_obj_set_size(ui.container, LV_PCT(100), LV_PCT(100));
    lv_obj_set_style_bg_color(ui.container, lv_color_hex(0x000000), LV_PART_MAIN);
    lv_obj_clear_flag(ui.container, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_scrollbar_mode(ui.container, LV_SCROLLBAR_MODE_OFF);

    view_navigation_init(ui.container, "Settings", "Restore Network");

    memset(&cfg, 0, sizeof(cfg));
    cfg.action_text = "Restore Network defaults";
    cfg.btn_text = "Restore Network";
    cfg.busy_text = "Restoring...";
    cfg.on_done = network_reset_hold_done;
    cfg.user_data = NULL;
    view_hold_action_init(&ui.hold, ui.container, &cfg);
}

void settings_network_reset_page_update(void)
{
    if(!ui.hold.completed && !ui.hold.is_busy && !ui.hold.is_holding) {
        view_hold_action_reset_idle(&ui.hold);
    }
}

void settings_network_reset_page_cleanup(void)
{
    if(current_parent) {
        network_reset_stop_result_timer();
        view_hold_action_cleanup(&ui.hold);
        lv_obj_del(ui.container);
        current_parent = NULL;
    }
}
