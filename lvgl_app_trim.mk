# Project-side LVGL source trimming (does not modify the lvgl submodule).
# Included from the top-level Makefile after lvgl.mk / lv_drivers.mk.

# lv_font.mk lists bare filenames; lv_conf.h only controls linkage.
FONT_DROP := \
	lv_font_dejavu_16_persian_hebrew.c \
	lv_font_montserrat_8.c \
	lv_font_montserrat_12.c \
	lv_font_montserrat_12_subpx.c \
	lv_font_montserrat_14.c \
	lv_font_montserrat_16.c \
	lv_font_montserrat_18.c \
	lv_font_montserrat_20.c \
	lv_font_montserrat_22.c \
	lv_font_montserrat_24.c \
	lv_font_montserrat_26.c \
	lv_font_montserrat_28.c \
	lv_font_montserrat_28_compressed.c \
	lv_font_montserrat_30.c \
	lv_font_montserrat_32.c \
	lv_font_montserrat_34.c \
	lv_font_montserrat_36.c \
	lv_font_montserrat_38.c \
	lv_font_montserrat_40.c \
	lv_font_montserrat_42.c \
	lv_font_montserrat_44.c \
	lv_font_montserrat_46.c \
	lv_font_montserrat_48.c \
	lv_font_simsun_16_cjk.c \
	lv_font_unscii_8.c \
	lv_font_unscii_16.c
CSRCS := $(filter-out $(FONT_DROP),$(CSRCS))

# LV_USE_GPU_SDL=0 — software renderer only (bare filenames from lv_draw_sdl.mk).
CSRCS := $(filter-out $(filter lv_draw_sdl%,$(CSRCS)),$(CSRCS))

# Core widgets disabled in lv_conf.h (bare filenames from lv_widgets.mk).
WIDGET_DROP := \
	lv_bar.c lv_btnmatrix.c lv_canvas.c lv_dropdown.c \
	lv_line.c lv_roller.c lv_slider.c lv_switch.c lv_table.c lv_textarea.c
CSRCS := $(filter-out $(WIDGET_DROP),$(CSRCS))

# Embedded target: fbdev + evdev only (full paths from lv_drivers.mk wildcards).
CSRCS := $(filter-out \
	$(wildcard $(LVGL_DIR)/lv_drivers/*.c) \
	$(wildcard $(LVGL_DIR)/lv_drivers/wayland/*.c) \
	$(wildcard $(LVGL_DIR)/lv_drivers/gtkdrv/*.c) \
	$(wildcard $(LVGL_DIR)/lv_drivers/display/*.c) \
	$(wildcard $(LVGL_DIR)/lv_drivers/indev/*.c) \
	,$(CSRCS))
CSRCS += $(LVGL_DIR)/lv_drivers/display/fbdev.c
CSRCS += $(LVGL_DIR)/lv_drivers/indev/evdev.c

# Optional codecs / FS drivers (all disabled in lv_conf.h).
CSRCS := $(filter-out \
	$(wildcard $(LVGL_DIR)/lvgl/src/extra/libs/qrcode/*.c) \
	$(wildcard $(LVGL_DIR)/lvgl/src/extra/libs/gif/*.c) \
	$(wildcard $(LVGL_DIR)/lvgl/src/extra/libs/png/*.c) \
	$(wildcard $(LVGL_DIR)/lvgl/src/extra/libs/bmp/*.c) \
	$(wildcard $(LVGL_DIR)/lvgl/src/extra/libs/sjpg/*.c) \
	$(wildcard $(LVGL_DIR)/lvgl/src/extra/libs/ffmpeg/*.c) \
	$(wildcard $(LVGL_DIR)/lvgl/src/extra/libs/freetype/*.c) \
	$(wildcard $(LVGL_DIR)/lvgl/src/extra/libs/rlottie/*.c) \
	$(wildcard $(LVGL_DIR)/lvgl/src/extra/libs/fsdrv/*.c) \
	,$(CSRCS))

# Extra widgets / themes / layouts disabled in lv_conf.h.
CSRCS := $(filter-out \
	$(wildcard $(LVGL_DIR)/lvgl/src/extra/widgets/*/*.c) \
	$(LVGL_DIR)/lvgl/src/extra/themes/basic/lv_theme_basic.c \
	$(LVGL_DIR)/lvgl/src/extra/themes/mono/lv_theme_mono.c \
	$(LVGL_DIR)/lvgl/src/extra/layouts/grid/lv_grid.c \
	,$(CSRCS))
