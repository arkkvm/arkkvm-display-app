#include "font_util.h"

lv_font_t * font_sls(FsFontType type)
{

    bool useFs = true;
    switch(type) {
        case FS_FONT_REGULAR_10: return useFs ? &lv_font_fs_regular_10 : &lv_font_montserrat_10;
        case FS_FONT_REGULAR_14:
            return &lv_font_fs_regular_14;
        case FS_FONT_REGULAR_16:
            return &lv_font_fs_regular_16;
        case FS_FONT_REGULAR_18:
            return &lv_font_fs_regular_18;
        case FS_FONT_REGULAR_20:
            return &lv_font_fs_regular_20;
        case FS_FONT_REGULAR_24:
            return &lv_font_fs_regular_24;
        case FS_FONT_REGULAR_30:
            return &lv_font_fs_regular_30;
        case FS_FONT_REGULAR_40:
            return &lv_font_fs_regular_40;
        case FS_FONT_MEDIUM_10:
            return &lv_font_fs_medium_10;
        case FS_FONT_MEDIUM_12:
            return &lv_font_fs_medium_12;
        case FS_FONT_MEDIUM_22:
            return &lv_font_fs_medium_22;
        case FS_FONT_MEDIUM_28:
            return &lv_font_fs_medium_28;
        case FS_FONT_SEMIBOLD_14:
            return &lv_font_fs_semibold_14;
        default: return &lv_font_montserrat_10;
    }
}
