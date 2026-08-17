#ifndef COMMAND_H
#define COMMAND_H
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

int run_command(const char* cmd, char* result, size_t result_size);
int run_command_no_result(const char* cmd);
int update_screen_luminance();
int sync_uboot_disp_env(int orientation, int screen_luminance);
#ifdef __cplusplus
}
#endif

#endif