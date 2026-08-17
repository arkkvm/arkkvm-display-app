#include "settings_reboot_page.h"
#include "../components/view_navigation.h"
#include "../utils/command.h"
#include "../utils/view_util.h"
#include <string.h>

static SettingsRebootPageUI ui;
static lv_obj_t * current_parent = NULL;

static void reboot_hold_done(ViewHoldAction * action, void * user_data)
{
    LV_UNUSED(action);
    LV_UNUSED(user_data);
    /* Busy UI ("Rebooting...") already shown by view_hold_action. */
    run_command_no_result("reboot");
}

void settings_reboot_page_init(lv_obj_t * parent)
{
    ViewHoldActionConfig cfg;

    current_parent = parent;

    ui.container = are_create_view(parent);
    lv_obj_set_size(ui.container, LV_PCT(100), LV_PCT(100));
    lv_obj_set_style_bg_color(ui.container, lv_color_hex(0x000000), LV_PART_MAIN);
    lv_obj_clear_flag(ui.container, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_scrollbar_mode(ui.container, LV_SCROLLBAR_MODE_OFF);

    view_navigation_init(ui.container, "Settings", "Reboot");

    memset(&cfg, 0, sizeof(cfg));
    cfg.action_text = "to Reboot";
    cfg.btn_text = "Reboot";
    cfg.busy_text = "Rebooting...";
    cfg.on_done = reboot_hold_done;
    cfg.user_data = NULL;
    view_hold_action_init(&ui.hold, ui.container, &cfg);
}

void settings_reboot_page_update(void)
{
    if(!ui.hold.completed && !ui.hold.is_busy && !ui.hold.is_holding) {
        view_hold_action_reset_idle(&ui.hold);
    }
}

void settings_reboot_page_cleanup(void)
{
    if(current_parent) {
        view_hold_action_cleanup(&ui.hold);
        lv_obj_del(ui.container);
        current_parent = NULL;
    }
}
