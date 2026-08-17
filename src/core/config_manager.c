#include "config_manager.h"
#include "page_manager.h"
#include "../utils/command.h"
#include "../utils/debug.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <pthread.h>

static pthread_mutex_t g_config_mutex = PTHREAD_MUTEX_INITIALIZER;
static UIConfig g_config = {
    .orientation      = 1,
    .sleep_time       = 1800,
    .screen_luminance = 64,
    .dark_screen_time = 0
};

static int create_config_dir(void)
{
    struct stat st = {0};

    if(stat(UI_CONFIG_DIR_PATH, &st) == -1) {
        if(mkdir(UI_CONFIG_DIR_PATH, 0755) == -1) {
            perror("Failed to create config directory");
            return -1;
        }
        DEBUG_LOG("Config directory created: %s\n", UI_CONFIG_DIR_PATH);
    }
    return 0;
}

static int compare_config(const UIConfig * a, const UIConfig * b)
{
    return (a->orientation == b->orientation) && (a->sleep_time == b->sleep_time) &&
           (a->screen_luminance == b->screen_luminance) && (a->dark_screen_time == b->dark_screen_time);
}

static void config_sync_uboot_env(void)
{
    pthread_mutex_lock(&g_config_mutex);
    int orientation      = g_config.orientation;
    int screen_luminance = g_config.screen_luminance;
    pthread_mutex_unlock(&g_config_mutex);

    sync_uboot_disp_env(orientation, screen_luminance);
}

static void config_reset_defaults(void)
{
    g_config.orientation      = 1;
    g_config.sleep_time       = 1800;
    g_config.screen_luminance = 64;
    g_config.dark_screen_time = 0;
}

static int create_default_config(void)
{
    DEBUG_LOG("Creating default configuration file: %s\n", UI_CONFIG_FILE_PATH);

    FILE * file = fopen(UI_CONFIG_FILE_PATH, "w");
    if(!file) {
        perror("Failed to create config file");
        return -1;
    }

    fprintf(file, "# arkkvm UI configuration\n");
    fprintf(file, "# Auto-generated default configuration\n\n");
    fprintf(file, "orientation=%d\n", g_config.orientation);
    fprintf(file, "sleep_time=%d\n", g_config.sleep_time);
    fprintf(file, "screen_luminance=%d\n", g_config.screen_luminance);
    fprintf(file, "dark_screen_time=%d\n", g_config.dark_screen_time);
    fprintf(file, "version=%d\n", CONFIG_VERSION);

    fclose(file);
    DEBUG_LOG("Default configuration file created successfully\n");
    return 0;
}

void config_init(void)
{
    if(create_config_dir() != 0) {
        DEBUG_LOG("Warning: Using in-memory defaults only\n");
        config_sync_uboot_env();
        return;
    }

    FILE * file = fopen(UI_CONFIG_FILE_PATH, "r");
    if(!file) {
        DEBUG_LOG("Config file not found, creating with defaults...\n");
        if(create_default_config() != 0) {
            DEBUG_LOG("Warning: Using in-memory defaults only\n");
        }
        config_sync_uboot_env();
        return;
    }

    char line[64];
    int config_loaded = 0;
    int config_version = 0;
    while(fgets(line, sizeof(line), file)) {
        if(line[0] == '#' || line[0] == '\n') {
            continue;
        }

        char key[32], value[32];
        if(sscanf(line, "%31[^=]=%31s", key, value) == 2) {
            config_loaded = 1;

            if(strcmp(key, "orientation") == 0) {
                g_config.orientation = atoi(value);
            } else if(strcmp(key, "sleep_time") == 0) {
                g_config.sleep_time = atoi(value);
            } else if(strcmp(key, "screen_luminance") == 0) {
                g_config.screen_luminance = atoi(value);
            } else if(strcmp(key, "dark_screen_time") == 0) {
                g_config.dark_screen_time = atoi(value);
            } else if(strcmp(key, "version") == 0) {
                config_version = atoi(value);
            }
        }
    }
    fclose(file);
    check_config_validity(config_version);

    if(config_loaded) {
        DEBUG_LOG("Configuration loaded successfully from %s\n", UI_CONFIG_FILE_PATH);
    }

    config_sync_uboot_env();
}

void config_apply_to_runtime(GlobalData * data)
{
    if(!data) {
        return;
    }

    pthread_mutex_lock(&g_config_mutex);
    data->orientation       = g_config.orientation;
    data->sleep_time        = g_config.sleep_time;
    data->screen_luminance  = g_config.screen_luminance;
    data->dark_screen_time  = g_config.dark_screen_time;
    pthread_mutex_unlock(&g_config_mutex);
}

int config_save_display_settings(const DisplaySettings * settings)
{
    if(!settings) {
        DEBUG_LOG("Error: DisplaySettings pointer is NULL\n");
        return -1;
    }

    pthread_mutex_lock(&g_config_mutex);

    if(compare_config(&g_config, settings)) {
        pthread_mutex_unlock(&g_config_mutex);
        return 0;
    }

    g_config.orientation      = settings->orientation;
    g_config.sleep_time       = settings->sleep_time;
    g_config.screen_luminance = settings->screen_luminance;
    g_config.dark_screen_time = settings->dark_screen_time;

    pthread_mutex_unlock(&g_config_mutex);

    DEBUG_LOG("Configuration changed, saving...\n");
    if(config_save() != 0) {
        return -1;
    }
    config_sync_uboot_env();
    return 1;
}

int config_update_from_global(GlobalData * g_data)
{
    DisplaySettings settings;

    if(!g_data) {
        DEBUG_LOG("Error: GlobalData pointer is NULL\n");
        return -1;
    }

    settings.orientation      = g_data->orientation;
    settings.sleep_time       = g_data->sleep_time;
    settings.screen_luminance = g_data->screen_luminance;
    settings.dark_screen_time = g_data->dark_screen_time;

    return config_save_display_settings(&settings);
}

UIConfig * config_get(void)
{
    return &g_config;
}

int config_save(void)
{
    create_config_dir();

    pthread_mutex_lock(&g_config_mutex);

    FILE * file = fopen(UI_CONFIG_FILE_PATH, "w");
    if(!file) {
        pthread_mutex_unlock(&g_config_mutex);
        perror("Failed to open config file for writing");
        return -1;
    }

    fprintf(file, "# arkkvm UI configuration\n");
    fprintf(file, "# Last updated configuration\n\n");
    fprintf(file, "orientation=%d\n", g_config.orientation);
    fprintf(file, "sleep_time=%d\n", g_config.sleep_time);
    fprintf(file, "screen_luminance=%d\n", g_config.screen_luminance);
    fprintf(file, "dark_screen_time=%d\n", g_config.dark_screen_time);
    fprintf(file, "version=%d\n", CONFIG_VERSION);

    fclose(file);
    pthread_mutex_unlock(&g_config_mutex);

    DEBUG_LOG("Configuration saved successfully to %s\n", UI_CONFIG_FILE_PATH);
    return 0;
}

void check_config_validity(int config_version)
{
    if(config_version == CONFIG_VERSION) {
        return;
    }

    if(config_version == 0) {
        DEBUG_LOG("Config file missing version, resetting to defaults\n");
    } else {
        DEBUG_LOG("Config version mismatch (file: %d, expected: %d), resetting to defaults\n",
               config_version, CONFIG_VERSION);
    }

    config_reset_defaults();
    config_save();
}
