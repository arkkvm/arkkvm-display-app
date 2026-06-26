#ifndef DATA_MANAGER_H
#define DATA_MANAGER_H

#include "page_manager.h"
#include <stdbool.h>
#include <time.h>

typedef void (*DataUpdateCallback)(void);

void data_manager_init(void);
void data_manager_update(void);
void data_manager_register_callback(DataUpdateCallback cb);
void data_manager_reset_poll_timer(void);
bool data_manager_get_network_status(void);
time_t data_manager_get_last_update_time(void);

#endif
