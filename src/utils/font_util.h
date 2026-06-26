#ifndef FONT_UTIL_H
#define FONT_UTIL_H 
#include "../../lvgl/lvgl.h"

#define LV_FONT_CUSTOM_DECLARE \
    LV_FONT_DECLARE(lv_font_fs_regular_10) \
    LV_FONT_DECLARE(lv_font_fs_regular_14) \
    LV_FONT_DECLARE(lv_font_fs_regular_16) \
    LV_FONT_DECLARE(lv_font_fs_medium_9)  \
    LV_FONT_DECLARE(lv_font_fs_medium_12)  \
    LV_FONT_DECLARE(lv_font_fs_medium_22)  \
    LV_FONT_DECLARE(lv_font_fs_medium_28)  \
    LV_FONT_DECLARE(lv_font_fs_semibold_14)

typedef enum{
    FS_FONT_REGULAR_10,
    FS_FONT_REGULAR_14,
    FS_FONT_REGULAR_16,
    FS_FONT_MEDIUM_10,
    FS_FONT_MEDIUM_12,
    FS_FONT_MEDIUM_22,
    FS_FONT_MEDIUM_28,
    FS_FONT_SEMIBOLD_14,
}FsFontType;


lv_font_t  *font_sls(FsFontType type);



#endif