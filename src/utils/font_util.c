#include "font_util.h"

lv_font_t * font_sls(FsFontType type)
{

    bool useFs = true;
    switch(type) {
        case FS_FONT_REGULAR_10: return useFs ? &lv_font_fs_regular_10 : &lv_font_montserrat_10;
        case FS_FONT_REGULAR_14:
            // return  useFs?&lv_font_fs_regular_14:&lv_font_montserrat_14;
            return &lv_font_fs_regular_14;
        case FS_FONT_REGULAR_16:
            // return  useFs?&lv_font_fs_regular_16:&lv_font_montserrat_16;
            return &lv_font_fs_regular_16;
        case FS_FONT_MEDIUM_10:
            // return  useFs?&lv_font_fs_medium_9:&lv_font_montserrat_10;
            return &lv_font_fs_medium_10;
        case FS_FONT_MEDIUM_12:
            // return  useFs?&lv_font_fs_medium_12:&lv_font_montserrat_12;
            return &lv_font_fs_medium_12;
        case FS_FONT_MEDIUM_22:
            // return  useFs?&lv_font_fs_medium_22:&lv_font_montserrat_22;
            return &lv_font_fs_medium_22;
        case FS_FONT_MEDIUM_28:
            // return  useFs?&lv_font_fs_medium_28:&lv_font_montserrat_28;
            return &lv_font_fs_medium_28;
        case FS_FONT_SEMIBOLD_14:
            // return  useFs?&lv_font_fs_semibold_14:&lv_font_montserrat_14;
            return &lv_font_fs_semibold_14;
        default: return &lv_font_montserrat_10;
    }
}