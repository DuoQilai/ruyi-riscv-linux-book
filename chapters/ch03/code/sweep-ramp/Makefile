# sweep-ramp — 第三章实验一：温度扫描边界
#
# 学生半成品：  make          → build/sweep-ramp
# 参考实现：    make sol      → build/sweep-ramp-sol
# 本机或板端（riscv64 gcc）均可。
#
#   make && ./build/sweep-ramp
#   make sol && ./build/sweep-ramp-sol

CROSS_COMPILE ?=
CC      := $(CROSS_COMPILE)gcc
CFLAGS  := -Wall -Wextra -O0 -g -Iinclude
LDFLAGS := -lm

BUILD_DIR   := build
TARGET      := $(BUILD_DIR)/sweep-ramp
SOL_TARGET  := $(BUILD_DIR)/sweep-ramp-sol

.PHONY: all sol clean dirs

all: dirs $(TARGET)
sol: dirs $(SOL_TARGET)

dirs:
	@mkdir -p $(BUILD_DIR)/src

$(TARGET): $(BUILD_DIR)/src/main.o $(BUILD_DIR)/src/ramp.o | dirs
	$(CC) $(LDFLAGS) -o $@ $^

$(SOL_TARGET): $(BUILD_DIR)/src/main.o $(BUILD_DIR)/src/ramp-sol.o | dirs
	$(CC) $(LDFLAGS) -o $@ $^

$(BUILD_DIR)/src/main.o: src/main.c include/ramp.h | dirs
	$(CC) $(CFLAGS) -c -o $@ $<

$(BUILD_DIR)/src/ramp.o: src/ramp.c include/ramp.h | dirs
	$(CC) $(CFLAGS) -c -o $@ $<

$(BUILD_DIR)/src/ramp-sol.o: src/ramp-sol.c include/ramp.h | dirs
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -rf $(BUILD_DIR)
