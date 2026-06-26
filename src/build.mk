# src/build.mk
PROJ_SRC_DIRS = core pages utils components
PROJ_CSRCS = $(foreach dir,$(PROJ_SRC_DIRS),$(wildcard src/$(dir)/*.c))
PROJ_INCS = $(foreach dir,$(PROJ_SRC_DIRS),-Isrc/$(dir))


CSRCS += $(PROJ_CSRCS)
INCS += $(PROJ_INCS)

# CSRCS += $(shell find -L $(LVGL_DIR)/src/ -name "*.c")