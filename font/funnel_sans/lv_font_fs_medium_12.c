/*******************************************************************************
 * Size: 12 px
 * Bpp: 2
 * Opts: --bpp 2 --size 12 --no-compress --use-color-info --stride 1 --align 1 --font FunnelSans-Medium.ttf --symbols abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789.,?!:;'"-—()[]{}.../@#$%&+=<>^|~* --format lvgl -o lv_font_fs_medium_12.c
 ******************************************************************************/

#ifdef __has_include
    #if __has_include("lvgl.h")
        #ifndef LV_LVGL_H_INCLUDE_SIMPLE
            #define LV_LVGL_H_INCLUDE_SIMPLE
        #endif
    #endif
#endif

#ifdef LV_LVGL_H_INCLUDE_SIMPLE
    #include "lvgl.h"
#else
    #include "lvgl/lvgl.h"
#endif



#ifndef LV_FONT_FS_MEDIUM_12
#define LV_FONT_FS_MEDIUM_12 1
#endif

#if LV_FONT_FS_MEDIUM_12

/*-----------------
 *    BITMAPS
 *----------------*/

/*Store the image of the glyphs*/
static LV_ATTRIBUTE_LARGE_CONST const uint8_t glyph_bitmap[] = {
    /* U+0021 "!" */
    0x34, 0xd3, 0x4c, 0x30, 0x42, 0xd,

    /* U+0022 "\"" */
    0x73, 0x1c, 0xc7, 0x30,

    /* U+0023 "#" */
    0xd, 0x70, 0x32, 0x87, 0xff, 0x46, 0x30, 0x24,
    0xc3, 0xff, 0xc3, 0x18, 0xc, 0x90,

    /* U+0024 "$" */
    0x7, 0x0, 0xbf, 0x43, 0x7, 0xe, 0x0, 0xb,
    0xd0, 0x41, 0xc7, 0x47, 0x7, 0xf4, 0x3, 0x0,

    /* U+0025 "%" */
    0x3e, 0xa, 0x6, 0x30, 0xc0, 0x63, 0x24, 0x2,
    0xd3, 0x0, 0x0, 0xd7, 0x80, 0x18, 0xc9, 0x3,
    0xc, 0xa0, 0x60, 0x7c,

    /* U+0026 "&" */
    0x1f, 0xc0, 0x38, 0x0, 0x34, 0x34, 0x1f, 0xfd,
    0x34, 0x38, 0x70, 0x34, 0x74, 0x34, 0x1f, 0xf4,

    /* U+0027 "'" */
    0x77, 0x70,

    /* U+0028 "(" */
    0x1c, 0x34, 0x70, 0x70, 0xa0, 0xa0, 0x70, 0x30,
    0x38, 0x8,

    /* U+0029 ")" */
    0x70, 0x38, 0x1c, 0xc, 0xc, 0xc, 0x1c, 0x2c,
    0x34, 0x60,

    /* U+002A "*" */
    0x6, 0x1, 0xa5, 0x2f, 0x90, 0xf4, 0x24, 0x80,

    /* U+002B "+" */
    0x1, 0x0, 0x1c, 0x0, 0x70, 0x1f, 0xfc, 0x7,
    0x0, 0x1c, 0x0,

    /* U+002C "," */
    0x21, 0xc2, 0x28,

    /* U+002D "-" */
    0xbd, 0x0,

    /* U+002E "." */
    0x21, 0xc0,

    /* U+002F "/" */
    0x3, 0x1, 0xc0, 0xd0, 0x30, 0x28, 0xd, 0x7,
    0x2, 0x80,

    /* U+0030 "0" */
    0x1f, 0x80, 0xe3, 0x87, 0x7, 0x1c, 0x1c, 0x70,
    0x71, 0xc1, 0xc3, 0x8e, 0x7, 0xe0,

    /* U+0031 "1" */
    0xb, 0x43, 0xf4, 0x23, 0x40, 0x34, 0x3, 0x40,
    0x34, 0x3, 0x43, 0xff,

    /* U+0032 "2" */
    0x1f, 0x83, 0x4e, 0x20, 0x70, 0xd, 0x3, 0x80,
    0xe0, 0x3c, 0x7, 0xff,

    /* U+0033 "3" */
    0x1f, 0xc0, 0xd2, 0x80, 0xa, 0x2, 0xf0, 0x0,
    0xa0, 0x41, 0xc7, 0x4b, 0x7, 0xf0,

    /* U+0034 "4" */
    0x1, 0xd0, 0xf, 0x40, 0xdd, 0xa, 0x34, 0x70,
    0xd2, 0xff, 0xd0, 0xd, 0x0, 0x34,

    /* U+0035 "5" */
    0x3f, 0xe0, 0xd0, 0x3, 0x0, 0xf, 0xf0, 0x24,
    0xb0, 0x1, 0xc7, 0x4b, 0xb, 0xe0,

    /* U+0036 "6" */
    0xf, 0xc0, 0xe2, 0xc7, 0x0, 0x1e, 0xf0, 0x74,
    0xb1, 0xc0, 0xc3, 0x8b, 0x3, 0xf0,

    /* U+0037 "7" */
    0xbf, 0xf0, 0x2, 0xc0, 0xd, 0x0, 0x70, 0x3,
    0x40, 0x1c, 0x0, 0xe0, 0x7, 0x0,

    /* U+0038 "8" */
    0x1f, 0xc0, 0xd2, 0x83, 0xa, 0xb, 0xf0, 0x34,
    0xb1, 0xc1, 0xc7, 0x4b, 0x7, 0xf0,

    /* U+0039 "9" */
    0x1f, 0x87, 0x4e, 0x70, 0x77, 0xb, 0x2f, 0xb0,
    0xb, 0x74, 0xd2, 0xf8,

    /* U+003A ":" */
    0x34, 0x90, 0x9, 0x34,

    /* U+003B ";" */
    0x34, 0x90, 0x9, 0x34, 0x57, 0x0,

    /* U+003C "<" */
    0x0, 0x10, 0x6e, 0x3d, 0x3, 0xd0, 0x7, 0xe0,
    0x2,

    /* U+003D "=" */
    0x7f, 0xf0, 0x0, 0x7f, 0xf0, 0x0,

    /* U+003E ">" */
    0x10, 0x3, 0xe0, 0x2, 0xe0, 0x2e, 0x2e, 0x1,
    0x0,

    /* U+003F "?" */
    0x2f, 0x4b, 0x1c, 0x1, 0xc0, 0x74, 0xd, 0x0,
    0x40, 0x8, 0x1, 0xc0,

    /* U+0040 "@" */
    0x2, 0xfe, 0x0, 0x34, 0x7, 0x43, 0x5f, 0xe3,
    0xc, 0xe3, 0x8c, 0x63, 0xa, 0x31, 0xcd, 0x29,
    0xc3, 0x4f, 0x7c, 0x7, 0x40, 0x0, 0x2, 0xff,
    0x0,

    /* U+0041 "A" */
    0x3, 0xc0, 0x7, 0xc0, 0xe, 0xa0, 0xc, 0x70,
    0x2c, 0x34, 0x3f, 0xfc, 0x74, 0x1c, 0xa0, 0xe,

    /* U+0042 "B" */
    0xff, 0xc3, 0x42, 0xcd, 0xa, 0x3f, 0xf0, 0xd0,
    0xb3, 0x40, 0xcd, 0xb, 0x3f, 0xf4,

    /* U+0043 "C" */
    0xb, 0xf4, 0x2d, 0x2c, 0x34, 0x9, 0x70, 0x0,
    0x70, 0x0, 0x34, 0x9, 0x2d, 0x2c, 0xb, 0xf4,

    /* U+0044 "D" */
    0xff, 0x83, 0x47, 0xcd, 0x3, 0x74, 0xa, 0xd0,
    0x2b, 0x40, 0xdd, 0x1f, 0x3f, 0xe0,

    /* U+0045 "E" */
    0xff, 0xdd, 0x0, 0xd0, 0xf, 0xfc, 0xd0, 0xd,
    0x0, 0xd0, 0xf, 0xfd,

    /* U+0046 "F" */
    0xff, 0xcd, 0x0, 0xd0, 0xf, 0xfc, 0xd0, 0xd,
    0x0, 0xd0, 0xd, 0x0,

    /* U+0047 "G" */
    0xb, 0xf4, 0x2d, 0x2d, 0x34, 0x9, 0x70, 0x0,
    0x70, 0xbd, 0x34, 0xa, 0x2d, 0x1e, 0xb, 0xea,

    /* U+0048 "H" */
    0xd0, 0x3b, 0x40, 0xed, 0x3, 0xbf, 0xfe, 0xd0,
    0x3b, 0x40, 0xed, 0x3, 0xb4, 0xe,

    /* U+0049 "I" */
    0xdd, 0xdd, 0xdd, 0xdd,

    /* U+004A "J" */
    0xe, 0xe, 0xe, 0xe, 0xe, 0xe, 0xe, 0xbd,

    /* U+004B "K" */
    0xd0, 0xb3, 0x4b, 0xd, 0xb0, 0x3f, 0x40, 0xfb,
    0x3, 0x4e, 0xd, 0xd, 0x34, 0x1c,

    /* U+004C "L" */
    0xd0, 0xd, 0x0, 0xd0, 0xd, 0x0, 0xd0, 0xd,
    0x0, 0xd0, 0xf, 0xfc,

    /* U+004D "M" */
    0xf0, 0xb, 0x7d, 0x3, 0xde, 0xc1, 0xf7, 0x70,
    0xdd, 0xda, 0x33, 0x75, 0xe8, 0xdd, 0x3d, 0x37,
    0x4b, 0xd,

    /* U+004E "N" */
    0xf0, 0x1f, 0xd0, 0x7e, 0xc1, 0xf6, 0xc7, 0xd3,
    0x9f, 0x47, 0xbd, 0xb, 0xf4, 0xf,

    /* U+004F "O" */
    0xb, 0xf4, 0xb, 0x4b, 0x43, 0x40, 0xb1, 0xc0,
    0x1c, 0x70, 0x7, 0xd, 0x2, 0xc2, 0xd2, 0xd0,
    0x2f, 0xd0,

    /* U+0050 "P" */
    0xff, 0xcd, 0xf, 0xd0, 0x7d, 0xb, 0xff, 0xdd,
    0x0, 0xd0, 0xd, 0x0,

    /* U+0051 "Q" */
    0xb, 0xf4, 0xb, 0x4b, 0x43, 0x40, 0xb1, 0xc0,
    0x1c, 0x70, 0x7, 0xc, 0x1, 0xc3, 0xc0, 0xe0,
    0x3f, 0xe0, 0x1, 0xc0, 0x0, 0x3f, 0x0, 0x0,
    0x0,

    /* U+0052 "R" */
    0xff, 0xd3, 0x42, 0xcd, 0x3, 0x34, 0x2c, 0xff,
    0xe3, 0x41, 0xcd, 0x3, 0x34, 0xc,

    /* U+0053 "S" */
    0x1f, 0xe0, 0xd1, 0xd3, 0x41, 0x4b, 0xe0, 0x1,
    0xb4, 0x80, 0xa3, 0x83, 0x87, 0xf8,

    /* U+0054 "T" */
    0xff, 0xf8, 0x1d, 0x0, 0x30, 0x0, 0xc0, 0x3,
    0x0, 0xc, 0x0, 0x30, 0x0, 0xc0,

    /* U+0055 "U" */
    0x34, 0xd, 0x34, 0xd, 0x34, 0xd, 0x34, 0xd,
    0x34, 0xd, 0x34, 0xd, 0x2d, 0x6c, 0xb, 0xf0,

    /* U+0056 "V" */
    0xb0, 0xe, 0x70, 0xc, 0x38, 0x2c, 0x2c, 0x34,
    0xc, 0x70, 0xe, 0xa0, 0x7, 0xd0, 0x3, 0xc0,

    /* U+0057 "W" */
    0xb0, 0x74, 0x39, 0xc2, 0xe0, 0xc3, 0x4e, 0xc7,
    0xa, 0x33, 0x28, 0x1d, 0x8d, 0xd0, 0x39, 0x2b,
    0x0, 0xf0, 0x7c, 0x2, 0xc0, 0xe0,

    /* U+0058 "X" */
    0x70, 0x2c, 0x2c, 0x70, 0xe, 0xd0, 0x7, 0xc0,
    0xb, 0xc0, 0xd, 0xe0, 0x38, 0x34, 0xb0, 0x2c,

    /* U+0059 "Y" */
    0xb0, 0x28, 0x34, 0x70, 0x1c, 0xe0, 0xf, 0xc0,
    0x7, 0x80, 0x3, 0x0, 0x3, 0x0, 0x3, 0x0,

    /* U+005A "Z" */
    0x7f, 0xf0, 0x3, 0xc0, 0x1c, 0x0, 0xd0, 0xf,
    0x0, 0x70, 0x3, 0x80, 0x2f, 0xfd,

    /* U+005B "[" */
    0x3e, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30,
    0x30, 0x3e, 0x0,

    /* U+005D "]" */
    0xbc, 0xc, 0xc, 0xc, 0xc, 0xc, 0xc, 0xc,
    0xc, 0xbc, 0x0,

    /* U+005E "^" */
    0x7, 0x0, 0x3e, 0x1, 0xcc, 0xd, 0x24, 0x30,
    0x70,

    /* U+0061 "a" */
    0x2f, 0x82, 0xd, 0x2f, 0xdb, 0xd, 0xb1, 0xd3,
    0xed,

    /* U+0062 "b" */
    0x30, 0x0, 0xc0, 0x3, 0xbd, 0xe, 0x2c, 0x30,
    0x34, 0xc0, 0xd3, 0x8b, 0xe, 0xf4,

    /* U+0063 "c" */
    0x1f, 0xc0, 0xd2, 0xc7, 0x0, 0x1c, 0x0, 0x38,
    0xb0, 0x7f, 0x0,

    /* U+0064 "d" */
    0x0, 0x30, 0x0, 0xc1, 0xfb, 0xd, 0x2c, 0x70,
    0x31, 0xc0, 0xc3, 0x4b, 0x7, 0xec,

    /* U+0065 "e" */
    0x1f, 0xc3, 0x4b, 0x7f, 0xf7, 0x0, 0x34, 0xb1,
    0xfc,

    /* U+0066 "f" */
    0x2f, 0x38, 0xff, 0x38, 0x38, 0x38, 0x38, 0x38,

    /* U+0067 "g" */
    0x1f, 0xb0, 0xe2, 0xc7, 0x3, 0x1c, 0xc, 0x38,
    0xb0, 0x7e, 0xc2, 0xb, 0xb, 0xf0,

    /* U+0068 "h" */
    0x30, 0x0, 0xc0, 0x3, 0xbd, 0xe, 0x2c, 0x30,
    0x30, 0xc0, 0xc3, 0x3, 0xc, 0xc,

    /* U+0069 "i" */
    0x30, 0x43, 0xc, 0x30, 0xc3, 0xc,

    /* U+006A "j" */
    0xc, 0x4, 0xc, 0xc, 0xc, 0xc, 0xc, 0xc,
    0xc, 0x3c,

    /* U+006B "k" */
    0x30, 0x0, 0xc0, 0x3, 0xe, 0xc, 0xe0, 0x3f,
    0x0, 0xee, 0x3, 0x1d, 0xc, 0x2c,

    /* U+006C "l" */
    0x30, 0xc3, 0xc, 0x30, 0xc3, 0xc,

    /* U+006D "m" */
    0x3b, 0xdb, 0xc3, 0x8b, 0x4e, 0x30, 0x70, 0xb3,
    0x7, 0xb, 0x30, 0x70, 0xb3, 0x7, 0xb,

    /* U+006E "n" */
    0x3b, 0xd0, 0xe2, 0xc3, 0x3, 0xc, 0xc, 0x30,
    0x30, 0xc0, 0xc0,

    /* U+006F "o" */
    0x1f, 0xc0, 0xe2, 0xc7, 0x3, 0x5c, 0xd, 0x38,
    0xb0, 0x7f, 0x0,

    /* U+0070 "p" */
    0x3b, 0xd0, 0xe1, 0xc3, 0x3, 0x4c, 0xd, 0x38,
    0xb0, 0xef, 0x43, 0x0, 0xc, 0x0,

    /* U+0071 "q" */
    0x1f, 0xb0, 0xd2, 0xc7, 0x3, 0x1c, 0xc, 0x34,
    0xb0, 0x7e, 0xc0, 0x3, 0x0, 0xc,

    /* U+0072 "r" */
    0x3b, 0x4e, 0x3, 0x40, 0xd0, 0x34, 0xd, 0x0,

    /* U+0073 "s" */
    0x2f, 0x87, 0xc, 0x3e, 0x0, 0x6c, 0x70, 0xd2,
    0xf8,

    /* U+0074 "t" */
    0x10, 0x34, 0xff, 0x38, 0x34, 0x34, 0x38, 0x2f,

    /* U+0075 "u" */
    0x30, 0x30, 0xc0, 0xc3, 0x3, 0xc, 0xc, 0x38,
    0xb0, 0x7d, 0xc0,

    /* U+0076 "v" */
    0xa0, 0x71, 0xc3, 0x83, 0x4c, 0xa, 0x70, 0xf,
    0x40, 0x3c, 0x0,

    /* U+0077 "w" */
    0xa0, 0xe0, 0xc7, 0x1f, 0x1c, 0x32, 0xb2, 0x83,
    0xb2, 0xb4, 0x2f, 0x1f, 0x0, 0xe0, 0xe0,

    /* U+0078 "x" */
    0x70, 0xe2, 0xec, 0xf, 0x0, 0xf4, 0x39, 0xc7,
    0xe,

    /* U+0079 "y" */
    0xa0, 0x71, 0xc3, 0x83, 0x4c, 0x7, 0xa0, 0xf,
    0x40, 0x2c, 0x0, 0xe0, 0x1f, 0x0,

    /* U+007A "z" */
    0x7f, 0xc0, 0x38, 0xb, 0x1, 0xc0, 0x34, 0xb,
    0xfc,

    /* U+007B "{" */
    0xb, 0x7, 0x1, 0xc0, 0x70, 0xb4, 0xb, 0x1,
    0xc0, 0x70, 0x1c, 0x3, 0xc0, 0x0,

    /* U+007C "|" */
    0xaa, 0xaa, 0xaa, 0xaa, 0xaa,

    /* U+007D "}" */
    0xb4, 0xb, 0x1, 0xc0, 0x70, 0xf, 0x7, 0x41,
    0xc0, 0x70, 0x2c, 0x2d, 0x0, 0x0,

    /* U+007E "~" */
    0x3d, 0x15, 0x7e, 0x0, 0x0,

    /* U+2014 "—" */
    0xbf, 0xff, 0xfe, 0x0, 0x0, 0x0
};


/*---------------------
 *  GLYPH DESCRIPTION
 *--------------------*/

static const lv_font_fmt_txt_glyph_dsc_t glyph_dsc[] = {
    {.bitmap_index = 0, .adv_w = 0, .box_w = 0, .box_h = 0, .ofs_x = 0, .ofs_y = 0} /* id = 0 reserved */,
    {.bitmap_index = 0, .adv_w = 51, .box_w = 3, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 6, .adv_w = 78, .box_w = 5, .box_h = 3, .ofs_x = 0, .ofs_y = 5},
    {.bitmap_index = 10, .adv_w = 106, .box_w = 7, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 24, .adv_w = 106, .box_w = 7, .box_h = 9, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 40, .adv_w = 161, .box_w = 10, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 60, .adv_w = 125, .box_w = 8, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 76, .adv_w = 39, .box_w = 2, .box_h = 3, .ofs_x = 0, .ofs_y = 5},
    {.bitmap_index = 78, .adv_w = 57, .box_w = 4, .box_h = 10, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 88, .adv_w = 57, .box_w = 4, .box_h = 10, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 98, .adv_w = 99, .box_w = 6, .box_h = 5, .ofs_x = 0, .ofs_y = 3},
    {.bitmap_index = 106, .adv_w = 106, .box_w = 7, .box_h = 6, .ofs_x = 0, .ofs_y = 1},
    {.bitmap_index = 117, .adv_w = 45, .box_w = 3, .box_h = 4, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 120, .adv_w = 61, .box_w = 4, .box_h = 2, .ofs_x = 0, .ofs_y = 2},
    {.bitmap_index = 122, .adv_w = 45, .box_w = 3, .box_h = 2, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 124, .adv_w = 73, .box_w = 5, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 134, .adv_w = 106, .box_w = 7, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 148, .adv_w = 106, .box_w = 6, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 160, .adv_w = 106, .box_w = 6, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 172, .adv_w = 106, .box_w = 7, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 186, .adv_w = 106, .box_w = 7, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 200, .adv_w = 106, .box_w = 7, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 214, .adv_w = 106, .box_w = 7, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 228, .adv_w = 106, .box_w = 7, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 242, .adv_w = 106, .box_w = 7, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 256, .adv_w = 106, .box_w = 6, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 268, .adv_w = 54, .box_w = 3, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 272, .adv_w = 54, .box_w = 3, .box_h = 7, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 278, .adv_w = 106, .box_w = 6, .box_h = 6, .ofs_x = 0, .ofs_y = 1},
    {.bitmap_index = 287, .adv_w = 106, .box_w = 6, .box_h = 4, .ofs_x = 0, .ofs_y = 2},
    {.bitmap_index = 293, .adv_w = 106, .box_w = 6, .box_h = 6, .ofs_x = 0, .ofs_y = 1},
    {.bitmap_index = 302, .adv_w = 89, .box_w = 6, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 314, .adv_w = 173, .box_w = 11, .box_h = 9, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 339, .adv_w = 124, .box_w = 8, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 355, .adv_w = 125, .box_w = 7, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 369, .adv_w = 127, .box_w = 8, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 385, .adv_w = 132, .box_w = 7, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 399, .adv_w = 110, .box_w = 6, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 411, .adv_w = 104, .box_w = 6, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 423, .adv_w = 136, .box_w = 8, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 439, .adv_w = 137, .box_w = 7, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 453, .adv_w = 55, .box_w = 2, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 457, .adv_w = 74, .box_w = 4, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 465, .adv_w = 123, .box_w = 7, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 479, .adv_w = 105, .box_w = 6, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 491, .adv_w = 167, .box_w = 9, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 509, .adv_w = 142, .box_w = 7, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 523, .adv_w = 139, .box_w = 9, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 541, .adv_w = 118, .box_w = 6, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 553, .adv_w = 139, .box_w = 9, .box_h = 11, .ofs_x = 0, .ofs_y = -3},
    {.bitmap_index = 578, .adv_w = 129, .box_w = 7, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 592, .adv_w = 117, .box_w = 7, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 606, .adv_w = 110, .box_w = 7, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 620, .adv_w = 133, .box_w = 8, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 636, .adv_w = 126, .box_w = 8, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 652, .adv_w = 176, .box_w = 11, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 674, .adv_w = 120, .box_w = 8, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 690, .adv_w = 114, .box_w = 8, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 706, .adv_w = 109, .box_w = 7, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 720, .adv_w = 64, .box_w = 4, .box_h = 11, .ofs_x = 0, .ofs_y = -3},
    {.bitmap_index = 731, .adv_w = 64, .box_w = 4, .box_h = 11, .ofs_x = 0, .ofs_y = -3},
    {.bitmap_index = 742, .adv_w = 106, .box_w = 7, .box_h = 5, .ofs_x = 0, .ofs_y = 1},
    {.bitmap_index = 751, .adv_w = 101, .box_w = 6, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 760, .adv_w = 111, .box_w = 7, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 774, .adv_w = 100, .box_w = 7, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 785, .adv_w = 111, .box_w = 7, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 799, .adv_w = 104, .box_w = 6, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 808, .adv_w = 64, .box_w = 4, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 816, .adv_w = 112, .box_w = 7, .box_h = 8, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 830, .adv_w = 112, .box_w = 7, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 844, .adv_w = 49, .box_w = 3, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 850, .adv_w = 48, .box_w = 4, .box_h = 10, .ofs_x = -1, .ofs_y = -2},
    {.bitmap_index = 860, .adv_w = 104, .box_w = 7, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 874, .adv_w = 49, .box_w = 3, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 880, .adv_w = 169, .box_w = 10, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 895, .adv_w = 112, .box_w = 7, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 906, .adv_w = 109, .box_w = 7, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 917, .adv_w = 111, .box_w = 7, .box_h = 8, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 931, .adv_w = 111, .box_w = 7, .box_h = 8, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 945, .adv_w = 75, .box_w = 5, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 953, .adv_w = 93, .box_w = 6, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 962, .adv_w = 67, .box_w = 4, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 970, .adv_w = 112, .box_w = 7, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 981, .adv_w = 99, .box_w = 7, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 992, .adv_w = 152, .box_w = 10, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1007, .adv_w = 99, .box_w = 6, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1016, .adv_w = 99, .box_w = 7, .box_h = 8, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 1030, .adv_w = 90, .box_w = 6, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1039, .adv_w = 72, .box_w = 5, .box_h = 11, .ofs_x = 0, .ofs_y = -3},
    {.bitmap_index = 1053, .adv_w = 96, .box_w = 2, .box_h = 10, .ofs_x = 2, .ofs_y = -2},
    {.bitmap_index = 1058, .adv_w = 72, .box_w = 5, .box_h = 11, .ofs_x = 0, .ofs_y = -3},
    {.bitmap_index = 1072, .adv_w = 106, .box_w = 6, .box_h = 3, .ofs_x = 0, .ofs_y = 2},
    {.bitmap_index = 1077, .adv_w = 192, .box_w = 12, .box_h = 2, .ofs_x = 0, .ofs_y = 2}
};

/*---------------------
 *  CHARACTER MAPPING
 *--------------------*/



/*Collect the unicode lists and glyph_id offsets*/
static const lv_font_fmt_txt_cmap_t cmaps[] =
{
    {
        .range_start = 33, .range_length = 59, .glyph_id_start = 1,
        .unicode_list = NULL, .glyph_id_ofs_list = NULL, .list_length = 0, .type = LV_FONT_FMT_TXT_CMAP_FORMAT0_TINY
    },
    {
        .range_start = 93, .range_length = 2, .glyph_id_start = 60,
        .unicode_list = NULL, .glyph_id_ofs_list = NULL, .list_length = 0, .type = LV_FONT_FMT_TXT_CMAP_FORMAT0_TINY
    },
    {
        .range_start = 97, .range_length = 30, .glyph_id_start = 62,
        .unicode_list = NULL, .glyph_id_ofs_list = NULL, .list_length = 0, .type = LV_FONT_FMT_TXT_CMAP_FORMAT0_TINY
    },
    {
        .range_start = 8212, .range_length = 1, .glyph_id_start = 92,
        .unicode_list = NULL, .glyph_id_ofs_list = NULL, .list_length = 0, .type = LV_FONT_FMT_TXT_CMAP_FORMAT0_TINY
    }
};

/*-----------------
 *    KERNING
 *----------------*/


/*Map glyph_ids to kern left classes*/
static const uint8_t kern_left_class_mapping[] =
{
    0, 1, 2, 0, 0, 0, 0, 2,
    0, 0, 0, 0, 3, 4, 3, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 5, 6, 7, 8, 9, 10, 11,
    12, 12, 12, 13, 14, 12, 12, 8,
    15, 8, 16, 17, 18, 19, 20, 20,
    13, 21, 22, 0, 0, 0, 23, 24,
    25, 12, 26, 27, 28, 29, 12, 12,
    30, 12, 29, 29, 24, 24, 31, 32,
    33, 34, 31, 35, 35, 30, 35, 36,
    0, 0, 0, 0, 4
};

/*Map glyph_ids to kern right classes*/
static const uint8_t kern_right_class_mapping[] =
{
    0, 1, 2, 0, 0, 0, 0, 2,
    0, 3, 4, 0, 5, 6, 5, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 4,
    0, 7, 8, 9, 8, 8, 8, 9,
    8, 8, 8, 8, 8, 8, 8, 9,
    8, 9, 8, 10, 11, 12, 13, 13,
    14, 15, 16, 0, 3, 0, 17, 18,
    19, 19, 19, 20, 21, 18, 18, 0,
    18, 18, 22, 22, 19, 22, 19, 22,
    23, 24, 25, 26, 26, 27, 26, 28,
    0, 0, 3, 0, 6
};

/*Kern values between classes*/
static const int8_t kern_class_values[] =
{
    10, 0, 0, 0, 5, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, -19, 0, -3, -3, 3, 0,
    0, -3, 0, 0, -5, 0, -8, 2,
    -8, 0, -8, 3, 0, 0, -3, 0,
    0, 0, 0, 0, 0, 0, 3, 5,
    -6, -3, -19, -5, -16, 0, -24, 3,
    0, 0, -3, 0, -3, 0, 0, -5,
    -3, -13, 0, 3, 0, 0, 0, 0,
    0, 0, -5, 0, 0, 0, -24, 0,
    -14, -11, -19, 0, 0, 0, 0, -3,
    0, 0, 0, 0, 0, -6, -11, -5,
    0, -19, 0, 0, 3, -5, 0, 0,
    -5, -2, -19, -3, -14, 0, -19, 0,
    0, 0, -2, 0, 0, 0, 0, 0,
    0, -8, 0, 0, 0, -5, 0, 0,
    0, 0, -3, 0, -3, 0, -5, 0,
    -5, -6, -10, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 5, 0, 0, -3, 0, -3, 0,
    0, 0, -5, 0, -3, -6, -6, -3,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, -5, 0, 0,
    -6, 0, -5, 0, 0, 0, -10, 0,
    -8, -13, -8, -6, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, -5, 0,
    0, 0, 0, 0, 0, -5, 0, 0,
    -3, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    -16, -3, -14, 0, -2, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, -3, 0, 0, 0, -3, 0,
    0, -3, 0, 0, 3, 2, 0, 0,
    0, 0, -10, 0, -10, 0, -14, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, -3, 0,
    0, -3, 0, 0, 0, -11, 0, 0,
    -13, -10, -3, -5, -5, 0, 0, 0,
    -3, 0, -6, 0, -6, 0, -5, -5,
    -5, -10, 0, 0, 0, -19, 0, 0,
    3, -5, 0, 0, -6, -5, -24, -3,
    -19, 0, -24, 0, 0, 0, -3, -3,
    -3, 0, 0, -6, -5, -10, 0, 0,
    0, 0, 0, 0, -18, -6, -13, 0,
    0, 0, 0, 0, -3, -10, -6, 0,
    -3, 0, -5, 0, -5, 0, -5, 0,
    0, 0, -3, 0, 0, -5, 0, 0,
    0, 0, 0, 0, 0, 0, -6, 0,
    -5, 0, -10, 0, 0, 0, 0, 0,
    0, 0, -2, 0, 0, -2, -3, 0,
    0, -2, 0, 0, 0, 0, -3, 0,
    0, 0, -5, 0, -5, -5, -8, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, -3, 0, 0, 3, 0, 0,
    -19, -24, -19, 0, -10, -3, 0, 0,
    0, 0, 0, 0, -16, 0, -16, -3,
    -16, -14, -16, 0, -16, -16, -16, -16,
    0, 0, 0, 0, -5, 0, -3, 0,
    0, 0, 0, 0, 0, -5, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    -16, -14, -14, 0, -8, -6, 0, 0,
    0, 0, 0, 0, -13, 0, -14, 0,
    -14, -11, -14, 0, -14, -6, -14, -10,
    0, 0, 0, 0, -24, -19, -19, 0,
    -8, -8, 0, 0, 0, 0, 0, 0,
    -16, 0, -16, -3, -16, -13, -18, 0,
    -11, -10, -16, -14, 0, 0, 0, 0,
    3, -5, 0, 0, -6, 0, 0, 0,
    0, 0, 0, 0, 0, 0, -3, 0,
    -3, 0, 0, -3, -3, -8, 0, 0,
    0, -11, 0, 0, 0, 0, 0, 0,
    0, 0, -16, 0, -13, 0, -16, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, -5, -5, 0, 0, -8, 0, 0,
    -3, 0, 0, 0, 0, 0, -18, 0,
    -14, -6, -16, -3, 0, 0, 0, 0,
    0, 0, 0, 0, 0, -3, -5, 0,
    0, -5, 0, 0, 0, 0, 0, 0,
    0, 0, -16, 0, -11, -5, -16, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, -2, -5, -2, 0, -8, 0, 0,
    0, 0, 0, 0, 0, 0, -16, 0,
    -16, -6, -16, -3, 0, 0, 0, 0,
    0, 0, 0, 0, 0, -3, -5, 0,
    5, 3, 3, 5, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, -3, 0, 0,
    0, 0, 0, 0, 0, 0, -16, 0,
    -14, 0, -16, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, -3, 0,
    0, -10, 0, 0, 0, 0, 0, 0,
    0, 0, -16, 0, -14, 0, -16, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, -3, -3, 0, 0, -3, 0, 0,
    0, -11, 0, -3, -5, -3, -16, 0,
    -14, 0, -16, 0, -3, -3, -5, -3,
    -8, -3, -6, 0, -5, -3, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, -16, 0, -11, 0, -13, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, -3, 0, 0, 0, 0, 0,
    -13, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, -3, 0, -5, 0,
    -5, 0, -3, 0, 0, 0, 0, 0,
    0, -6, 0, 0, 0, 0, 0, 0,
    0, 0, -16, 0, -14, -3, -14, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, -6, 0, 0, 0, 0, 0, 0,
    3, -6, 0, 0, 0, 0, 0, 0,
    -3, 2, 0, 2, 0, 0, 0, 0,
    0, 0, 0, 0, 0, -3, 0, 8,
    0, 0, 0, 0, -13, -6, -8, 0,
    0, 0, -16, 0, -6, -10, -10, -8,
    -3, 0, -3, 0, -5, 0, -6, 0,
    0, 0, -3, 0, 0, 0, 0, 0,
    3, -6, 0, 0, 0, 0, -16, 0,
    -10, 0, -11, 0, 0, 0, -3, 0,
    0, 0, 0, 0, 0, 0, 0, 0
};


/*Collect the kern class' data in one place*/
static const lv_font_fmt_txt_kern_classes_t kern_classes =
{
    .class_pair_values   = kern_class_values,
    .left_class_mapping  = kern_left_class_mapping,
    .right_class_mapping = kern_right_class_mapping,
    .left_class_cnt      = 36,
    .right_class_cnt     = 28,
};

/*--------------------
 *  ALL CUSTOM DATA
 *--------------------*/

#if LVGL_VERSION_MAJOR == 8
/*Store all the custom data of the font*/
static  lv_font_fmt_txt_glyph_cache_t cache;
#endif

#if LVGL_VERSION_MAJOR >= 8
static const lv_font_fmt_txt_dsc_t font_dsc = {
#else
static lv_font_fmt_txt_dsc_t font_dsc = {
#endif
    .glyph_bitmap = glyph_bitmap,
    .glyph_dsc = glyph_dsc,
    .cmaps = cmaps,
    .kern_dsc = &kern_classes,
    .kern_scale = 16,
    .cmap_num = 4,
    .bpp = 2,
    .kern_classes = 1,
    .bitmap_format = 0,
#if LVGL_VERSION_MAJOR == 8
    .cache = &cache
#endif

};

extern const lv_font_t lv_font_fs_medium_12;


/*-----------------
 *  PUBLIC FONT
 *----------------*/

/*Initialize a public general font descriptor*/
#if LVGL_VERSION_MAJOR >= 8
const lv_font_t lv_font_fs_medium_12 = {
#else
lv_font_t lv_font_fs_medium_12 = {
#endif
    .get_glyph_dsc = lv_font_get_glyph_dsc_fmt_txt,    /*Function pointer to get glyph's data*/
    .get_glyph_bitmap = lv_font_get_bitmap_fmt_txt,    /*Function pointer to get glyph's bitmap*/
    .line_height = 11,          /*The maximum line height required by the font*/
    .base_line = 3,             /*Baseline measured from the bottom of the line*/
#if !(LVGL_VERSION_MAJOR == 6 && LVGL_VERSION_MINOR == 0)
    .subpx = LV_FONT_SUBPX_NONE,
#endif
#if LV_VERSION_CHECK(7, 4, 0) || LVGL_VERSION_MAJOR >= 8
    .underline_position = -1,
    .underline_thickness = 1,
#endif
    // .static_bitmap = 0,
    .dsc = &font_dsc,          /*The custom font data. Will be accessed by `get_glyph_bitmap/dsc` */
// #if LV_VERSION_CHECK(8, 2, 0) || LVGL_VERSION_MAJOR >= 9
//     .fallback = &lv_font_fs_medium_12,
// #endif
    .user_data = NULL,
};



#endif /*#if LV_FONT_FS_MEDIUM_12*/
