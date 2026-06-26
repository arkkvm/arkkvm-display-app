#
# Makefile
#
#CC ?= gcc
BUILD ?= debug

#CC  = ~/arkkvm-devel/arm-rockchip830-linux-uclibcgnueabihf/bin/arm-rockchip830-linux-uclibcgnueabihf-gcc
CC = /opt/arkkvm-native-buildkit/bin/arm-rockchip830-linux-uclibcgnueabihf-cc


LVGL_DIR_NAME ?= lvgl
LVGL_DIR ?= ${shell pwd}
CFLAGS ?= -O3 -g0 -I$(LVGL_DIR)/ -Wall -Wshadow -Wundef -Wmissing-prototypes -Wno-discarded-qualifiers -Wall -Wextra -Wno-unused-function -Wno-error=strict-prototypes -Wpointer-arith -fno-strict-aliasing -Wno-error=cpp -Wuninitialized -Wmaybe-uninitialized -Wno-unused-parameter -Wno-missing-field-initializers -Wtype-limits -Wsizeof-pointer-memaccess -Wno-format-nonliteral -Wno-cast-qual -Wunreachable-code -Wno-switch-default -Wreturn-type -Wmultichar -Wformat-security -Wno-ignored-qualifiers -Wno-error=pedantic -Wno-sign-compare -Wno-error=missing-prototypes -Wdouble-promotion -Wclobbered -Wdeprecated -Wempty-body -Wtype-limits -Wstack-usage=2048 -Wno-unused-value -Wno-unused-parameter -Wno-missing-field-initializers -Wuninitialized -Wmaybe-uninitialized -Wall -Wextra -Wno-unused-parameter -Wno-missing-field-initializers -Wtype-limits -Wsizeof-pointer-memaccess -Wno-format-nonliteral -Wpointer-arith -Wno-cast-qual -Wmissing-prototypes -Wunreachable-code -Wno-switch-default -Wreturn-type -Wmultichar -Wno-discarded-qualifiers -Wformat-security -Wno-ignored-qualifiers -Wno-sign-compare
DEPFLAGS ?= -MMD -MP
LDFLAGS ?= -lm

BIN = bin/arkkvm_ui
BUILD_DIR ?= build

$(shell mkdir -p ${LVGL_DIR}/bin/ 2>/dev/null)
$(shell mkdir -p ${BUILD_DIR} 2>/dev/null)

#Collect the files to compile
MAINSRC = ./src/main.c

include $(LVGL_DIR)/lvgl/lvgl.mk
include $(LVGL_DIR)/src/build.mk
include $(LVGL_DIR)/img/ui/ui.mk
include $(LVGL_DIR)/lv_drivers/lv_drivers.mk
include $(LVGL_DIR)/font/funnel_sans/font.mk
include $(LVGL_DIR)/lvgl_app_trim.mk
# include $(LVGL_DIR)/lvgl/src/font/font.mk



CSRCS +=$(LVGL_DIR)/mouse_cursor_icon.c 
CSRCS +=$(LVGL_DIR)/top_img_dec.c

OBJEXT ?= .o

AOBJS = $(ASRCS:.S=$(OBJEXT))

# Map each .c to build/<relative-path>.o; strip $(LVGL_DIR)/ from absolute paths
# so build/ mirrors the source tree instead of embedding the host path.
src_relpath = $(patsubst $(LVGL_DIR)/%,%,$(1))
obj_from_src = $(BUILD_DIR)/$(patsubst %.c,%.o,$(call src_relpath,$(1)))
COBJS = $(foreach src,$(CSRCS),$(call obj_from_src,$(src)))
MAINOBJ = $(BUILD_DIR)/src/main.o
DEPS = $(COBJS:.o=.d) $(MAINOBJ:.o=.d)

SRCS = $(ASRCS) $(CSRCS) $(MAINSRC)
OBJS = $(AOBJS) $(COBJS)

## MAINOBJ -> OBJFILES

all: default

$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) $(DEPFLAGS) -c $< -o $@
	@echo "CC $<"
    
default: $(MAINOBJ) $(COBJS)
	$(CC) -o $(BIN) $(MAINOBJ) $(COBJS) $(LDFLAGS)

-include $(DEPS)

clean:
	rm -rf $(BUILD_DIR)
	rm -f $(BIN)
	@# legacy in-tree objects from pre-build/ Makefile
	find $(LVGL_DIR)/src $(LVGL_DIR)/lvgl $(LVGL_DIR)/lv_drivers \
		$(LVGL_DIR)/font $(LVGL_DIR)/img $(LVGL_DIR) -maxdepth 3 \
		-name '*.o' -type f -delete 2>/dev/null || true
