TARGET_EXEC ?= a.out

BUILD_DIR ?= ./build/bin
SRC_DIRS ?= \
	../_Official_Demos/EGUI_D4D_Demo/common_source \
	../_Official_Demos/EGUI_D4D_Demo/pc_demo \
	../D4D/common_files \
	../D4D/graphic_objects \
	../D4D/low_level_drivers/LCD/lcd_controllers_drivers/sdl \
	../D4D/low_level_drivers/touch_screen/touch_screen_drivers/sdl \


SRCS := $(shell find $(SRC_DIRS) -name *.cpp -or -name *.c -or -name *.s)
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

INC_DIRS := \
	../D4D \
	../_Official_Demos/EGUI_D4D_Demo/common_source \
	../_Official_Demos/EGUI_D4D_Demo/pc_demo \

INC_FLAGS := $(addprefix -I,$(INC_DIRS))

CFLAGS ?= $(INC_FLAGS) -MMD -MP -g3
LDFLAGS =  -L/usr/local/lib -lSDL2

$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS)
	@$(CC) $(OBJS) -o $@ $(LDFLAGS)

# c source
$(BUILD_DIR)/%.c.o: %.c
	@$(MKDIR_P) $(dir $@)
	@$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

.PHONY: clean
print-%  : ; @echo $* = $($*)

clean:
	$(RM) -r $(BUILD_DIR)

-include $(DEPS)

MKDIR_P ?= mkdir -p
