# virt-led-repl — 第五章实验二：虚拟灯命令环
CROSS_COMPILE ?=
CC      := $(CROSS_COMPILE)gcc
CFLAGS  := -Wall -Wextra -O0 -g -Iinclude
BUILD_DIR   := build
TARGET      := $(BUILD_DIR)/virt-led-repl
SOL_TARGET  := $(BUILD_DIR)/virt-led-repl-sol

.PHONY: all sol clean dirs
all: dirs $(TARGET)
sol: dirs $(SOL_TARGET)
dirs:
	@mkdir -p $(BUILD_DIR)/src
$(TARGET): $(BUILD_DIR)/src/main.o $(BUILD_DIR)/src/virt_led.o | dirs
	$(CC) -o $@ $^
$(SOL_TARGET): $(BUILD_DIR)/src/main.o $(BUILD_DIR)/src/virt_led-sol.o | dirs
	$(CC) -o $@ $^
$(BUILD_DIR)/src/%.o: src/%.c include/virt_led.h | dirs
	$(CC) $(CFLAGS) -c -o $@ $<
clean:
	rm -rf $(BUILD_DIR)
