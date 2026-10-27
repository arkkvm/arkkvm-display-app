#ifndef DEBUG_H
#define DEBUG_H

#include "file_log.h"
#include <stdbool.h>
#include <stdio.h>

extern bool g_debug_log;

#define DEBUG_LOG(...) do { \
    file_log_printf(__VA_ARGS__); \
    if(g_debug_log) { \
        printf(__VA_ARGS__); \
    } \
} while(0)

#endif
