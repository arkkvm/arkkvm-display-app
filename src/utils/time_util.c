#include "time_util.h"
#include "sys/time.h"
#include <stdio.h>
#include <stdlib.h> 

int64_t get_now_time(){
    struct timeval tv;
    gettimeofday(&tv, NULL);

    return (int64_t)tv.tv_sec;
}