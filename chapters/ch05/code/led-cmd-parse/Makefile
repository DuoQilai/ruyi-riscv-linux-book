# led-cmd-parse — 第五章实验一：on/off 精确解析
CROSS_COMPILE ?=
CC      := $(CROSS_COMPILE)gcc
CFLAGS  := -Wall -Wextra -O0 -g -Iinclude
BUILD_DIR   := build
TARGET      := $(BUILD_DIR)/led-cmd-parse
SOL_TARGET  := $(BUILD_DIR)/led-cmd-parse-sol

.PHONY: all sol clean dirs
all: dirs $(TARGET)
sol: dirs $(SOL_TARGET)
dirs:
	@mkdir -p $(BUILD_DIR)/src
$(TARGET): $(BUILD_DIR)/src/main.o $(BUILD_DIR)/src/led_cmd.o | dirs
	$(CC) -o $@ $^
$(SOL_TARGET): $(BUILD_DIR)/src/main.o $(BUILD_DIR)/src/led_cmd-sol.o | dirs
	$(CC) -o $@ $^
$(BUILD_DIR)/src/%.o: src/%.c include/led_cmd.h | dirs
	$(CC) $(CFLAGS) -c -o $@ $<
clean:
	rm -rf $(BUILD_DIR)
