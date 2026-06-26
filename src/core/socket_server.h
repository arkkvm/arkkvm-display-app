// core/socket_server.h
#ifndef SOCKET_SERVER_H
#define SOCKET_SERVER_H
#include <stddef.h>
#include <stdbool.h>
#include "lvgl/lvgl.h"
void ipc_server_start(const char* socket_path);
void ipc_server_stop(void);
bool ipc_send_command(const char* cmd, char* response, size_t resp_size);

#endif