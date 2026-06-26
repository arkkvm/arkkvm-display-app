#ifndef CONFIG_MANAGER_H
#define CONFIG_MANAGER_H

#define UI_CONFIG_DIR_PATH "/userdata/arkkvm/"
#define UI_CONFIG_FILE_PATH UI_CONFIG_DIR_PATH "ui_config.conf"
#define CONFIG_VERSION 1

typedef struct {
    int orientation;
    int sleep_time;
    int screen_luminance;
    int dark_screen_time;
} UIConfig;

typedef UIConfig DisplaySettings;

typedef struct GlobalData GlobalData;

void config_init(void);
void check_config_validity(int config_version);
UIConfig * config_get(void);
int config_save(void);

void config_apply_to_runtime(GlobalData * data);
int config_save_display_settings(const DisplaySettings * settings);

/** @deprecated Use config_save_display_settings; kept for existing call sites. */
int config_update_from_global(GlobalData * g_data);

#endif
