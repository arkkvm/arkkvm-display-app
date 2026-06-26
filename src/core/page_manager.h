#ifndef PAGE_MANAGER_H
#define PAGE_MANAGER_H

#include "../../lvgl/lvgl.h"
#include <stdbool.h>

// page types
typedef enum {
    PAGE_WEL= 0,
    PAGE_MAIN ,
    PAGE_SETTINGS,
    PAGE_SETTINGS_DISPLAY,
    PAGE_SETTINGS_ABOUT,
    PAGE_SETTINGS_REBOOT,
    PAGE_MAX
} PageType;

typedef struct {
    char vlan_tag[32];
    char ipv4[20];
    char ipv6[45];
    char mac[50];
    bool has_ipv6;
} GlobalVlanPage;

// global data shared across pages
typedef struct GlobalData {
    char ip[20];
    char ipv6[40];
    char mac[50];
    bool is_connect_net;
    bool is_connect_hdmi;
    bool is_connect_usb;
    int ui_screen_status;

    bool vlan_enabled;
    int primary_vlan_id;
    int secondary_vlan_id;
    int vlan_count;
    GlobalVlanPage vlan_pages[2];
    bool vlan_ipc_active;
  
    char app_version[100];
    char os_version[100];
    char serial_number[256];
    int sleep_time;
    int orientation;
    int dark_screen_time; //DARK_SCREEN_TIME
    int screen_luminance; //SCREEN_LUMINANCE
    uint32_t lastTouchTime;
    bool is_dark_screen;
    bool is_sleep;
} GlobalData;

// page function types
typedef void (*PageInitFunc)(lv_obj_t* container);
typedef void (*PageUpdateFunc)(void);
typedef void (*PageCleanupFunc)(void);

// page structure
typedef struct {
    PageType type;
    PageType ptype;
    const char* name;
    PageInitFunc init;
    PageUpdateFunc update;
    PageCleanupFunc cleanup;
    lv_obj_t* container;
    bool is_created;
} Page;

// page manager functions
void page_manager_init(void);
void page_switch(PageType page_type);
void page_back(void);
void page_update_all(void);



// macros for easier page operations
#define PAGE_SWITCH(page_type) page_switch(page_type)
#define PAGE_BACK() page_back()
#define PAGE_RELOAD() page_update_all()

GlobalData* page_get_global_data(void);

void set_tmp_ipv6_address(const char * ipv6);
char * get_ipv6_address(void);

#endif