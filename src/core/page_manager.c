#include "../core/page_manager.h"
#include "../core/config_manager.h"
#include "../pages/wel_page.h"
#include "../pages/main_page.h"
#include "../pages/settings_page.h"
#include "../pages/settings_display_page.h"
#include "../pages/settings_about_page.h"
#include "../pages/settings_reboot_page.h"
#include "../utils/view_util.h"
#include "../utils/command.h"
#include "../utils/time_util.h"
#include "../utils/debug.h"
#include <string.h>

// GlobalData static instance
static char tmp_ipv6[45];

static GlobalData g_global_data = {
    .ip = "",
    .mac = "",
    .is_connect_net = true,
    .is_connect_hdmi = false,
    .is_connect_usb = false,
    .ui_screen_status = 1,
    .vlan_enabled = false,
    .primary_vlan_id = 0,
    .secondary_vlan_id = 0,
    .vlan_count = 0,
    .vlan_ipc_active = false,
    .orientation = 1,
    .app_version = "",
    .os_version = "",
    .serial_number = "",
    .sleep_time = 1800,
    .dark_screen_time = 0,
    .screen_luminance = 64,
    .lastTouchTime = 0,
    .is_sleep = false,
    .is_dark_screen = false,
};



#define MAX_HISTORY 10
static PageType g_page_history[MAX_HISTORY];
static int g_history_index = -1;
static PageType g_current_page = PAGE_MAIN;

// page configurations
static Page g_pages[PAGE_MAX] = {
    [PAGE_WEL]={
        .type = PAGE_WEL,
        .name = "wel",
        .ptype = PAGE_MAX,
        .init = wel_page_init,
        .update = wel_page_update,
        .cleanup = wel_page_cleanup,
        .container = NULL,
        .is_created = false
    },
    [PAGE_MAIN] = {
        .type = PAGE_MAIN,
        .name = "main",
        .ptype = PAGE_MAX,
        .init = main_page_init,
        .update = main_page_update,
        .cleanup = main_page_cleanup,
        .container = NULL,
        .is_created = false
    },
    [PAGE_SETTINGS] = {
        .type = PAGE_SETTINGS,
        .ptype = PAGE_MAIN,
        .name = "settings",
        .init = settings_page_init,
        .update = settings_page_update,
        .cleanup = settings_page_cleanup,
        .container = NULL,
        .is_created = false
    },
     [PAGE_SETTINGS_DISPLAY] = {
        .ptype = PAGE_SETTINGS,
        .type = PAGE_SETTINGS_DISPLAY,
        .name = "settings_display",
        .init = settings_display_page_init,
        .update = settings_display_page_update,
        .cleanup = settings_display_page_cleanup,
        .container = NULL,
        .is_created = false
    },
     [PAGE_SETTINGS_ABOUT] = {
        .ptype = PAGE_SETTINGS,
        .type = PAGE_SETTINGS_ABOUT,
        .name = "settings_about",
        .init = settings_about_page_init,
        .update = settings_about_page_update,
        .cleanup = settings_about_page_cleanup,
        .container = NULL,
        .is_created = false
    },
     [PAGE_SETTINGS_REBOOT] = {
        .ptype = PAGE_SETTINGS,
        .type = PAGE_SETTINGS_REBOOT,
        .name = "settings_reboot",
        .init = settings_reboot_page_init,
        .update = settings_reboot_page_update,
        .cleanup = settings_reboot_page_cleanup,
        .container = NULL,
        .is_created = false
    }
};

static lv_timer_t *auto_switch_timer = NULL;
static void switch_to_main_page_cb(lv_timer_t * timer)
{
    (void)timer;
    page_switch(PAGE_MAIN);
   if(auto_switch_timer) {
        lv_timer_del(auto_switch_timer);
        auto_switch_timer = NULL;
    }
}

// init page manager
void page_manager_init(void) {
    
    memset(g_page_history, 0, sizeof(g_page_history));
    g_history_index = -1;
    GlobalData * data = page_get_global_data();
    config_apply_to_runtime(data);
    data->lastTouchTime = lv_tick_get();
    DEBUG_LOG("screen_luminance: %d, dark_screen_time: %d, sleep_time: %d\n",
           data->screen_luminance, data->dark_screen_time, data->sleep_time);

    lv_disp_set_rotation(lv_obj_get_disp(lv_scr_act()),
                        data->orientation == 1 ? LV_DISP_ROT_90 : LV_DISP_ROT_270);
    update_screen_luminance();
    // create and show the main page
    page_switch(PAGE_WEL);

    // Switch to PAGE_MAIN after 5 seconds without blocking the thread
    auto_switch_timer = lv_timer_create(switch_to_main_page_cb, 5000, NULL);
}

// switch to a specific page
void page_switch(PageType page_type) {
    if (page_type >= PAGE_MAX) return;

    // hide current page
    if (g_pages[g_current_page].is_created) {
        lv_obj_add_flag(g_pages[g_current_page].container, LV_OBJ_FLAG_HIDDEN);
    }  

    // update current page
    g_current_page = page_type;

    // initialize page if not created
    if (!g_pages[page_type].is_created) {
        g_pages[page_type].container = are_create_view(lv_scr_act()); 
 
        lv_obj_set_size(g_pages[page_type].container, LV_PCT(100), LV_PCT(100)); 
        lv_obj_clear_flag(g_pages[page_type].container, LV_OBJ_FLAG_SCROLLABLE);  // Disable scrolling
        g_pages[page_type].init(g_pages[page_type].container);
        g_pages[page_type].is_created = true;
    }

    // show the new page
    lv_obj_clear_flag(g_pages[page_type].container, LV_OBJ_FLAG_HIDDEN);
    
    // update the new page
    g_pages[page_type].update();
}

// go back to the previous page
void page_back(void) {
 

    PageType p_page =  g_pages[g_current_page].ptype;
    if(p_page != PAGE_MAX) {
         page_switch(p_page);
    }
}

// update all pages
void page_update_all(void) {
    for (int i = 0; i < PAGE_MAX; i++) {
        if (g_pages[i].is_created) {
            g_pages[i].update();
        }
    }
}

// get global data
GlobalData* page_get_global_data(void) {
    return &g_global_data;
}


void  set_tmp_ipv6_address(const char * ipv6){
    if(ipv6 == NULL || strlen(ipv6) == 0){
        tmp_ipv6[0] = '\0';  // Clear buffer
    }else{
        strncpy(tmp_ipv6, ipv6, sizeof(tmp_ipv6) - 1);
        tmp_ipv6[sizeof(tmp_ipv6) - 1] = '\0';  // Ensure null terminator
    } 

}
char * get_ipv6_address(void){
     if(tmp_ipv6[0] == '\0'){
        return NULL;
    }
    return tmp_ipv6;
}