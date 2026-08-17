#include "command.h"
#include "../utils/debug.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <errno.h>
#include <stdbool.h>
#include <sys/select.h>
#include <signal.h>
#include "../core/page_manager.h"

int run_command(const char * cmd, char * result, size_t result_size)
{
    FILE * fp = popen(cmd, "r");
    if(!fp) return -1;

    if(fgets(result, result_size, fp) == NULL) {
        pclose(fp);
        return -1;
    }

    // Strip trailing newline if present
    result[strcspn(result, "\r\n")] = '\0';
    pclose(fp);
    return 0;
}

int run_command_no_result(const char * cmd)
{
    int ret = system(cmd);
    if(ret == -1) {
        perror("system() failed");
        return -1;
    }
    return WEXITSTATUS(ret); // Return command exit status
}

int update_screen_luminance(){
    GlobalData *data =  page_get_global_data();
    if(data->screen_luminance == 0){
        set_screen_luminance(data->screen_luminance);
    }else if(data->is_sleep){
        DEBUG_LOG(">>> Setting brightness to 0 (sleep mode)\n");
        set_screen_luminance(0);
    }else if(data->is_dark_screen){
        set_screen_luminance(20);
        DEBUG_LOG(">>> Setting brightness to 20 (dark screen mode)\n");
    }else{
        set_screen_luminance(data->screen_luminance);
        DEBUG_LOG(">>> Restoring normal brightness: %d\n", data->screen_luminance);
    }
}


int set_screen_luminance(int value)
{
    char tmp[20];
    char command[256];
    snprintf(command, sizeof(command), "echo %d | tee /sys/class/backlight/backlight/brightness", value);
    DEBUG_LOG("command %s \n", command);
    run_command(command, tmp, sizeof(tmp));
}

int sync_uboot_disp_env(int orientation, int screen_luminance)
{
    int disp_orient = (orientation == 1) ? 1 : 2;
    char cmd[128];

    snprintf(cmd, sizeof(cmd), "fw_setenv disp_orient %d", disp_orient);
    if(run_command_no_result(cmd) != 0) {
        DEBUG_LOG("Failed to set disp_orient=%d\n", disp_orient);
        return -1;
    }

    snprintf(cmd, sizeof(cmd), "fw_setenv disp_bl %d", screen_luminance);
    if(run_command_no_result(cmd) != 0) {
        DEBUG_LOG("Failed to set disp_bl=%d\n", screen_luminance);
        return -1;
    }

    DEBUG_LOG("Synced uboot env: disp_orient=%d, disp_bl=%d\n", disp_orient, screen_luminance);
    return 0;
}
