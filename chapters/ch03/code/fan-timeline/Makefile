# fan-timeline — 第三章实验二：决策 + 时间线统计
#
# 学生半成品：  make          → build/fan-timeline
# 参考实现：    make sol      → build/fan-timeline-sol
# 本机或板端（riscv64 gcc）均可。
#
#   make && ./build/fan-timeline
#   make sol && ./build/fan-timeline-sol

CROSS_COMPILE ?=
CC      := $(CROSS_COMPILE)gcc
CFLAGS  := -Wall -Wextra -O0 -g -Iinclude
LDFLAGS :=

BUILD_DIR   := build
TARGET      := $(BUILD_DIR)/fan-timeline
SOL_TARGET  := $(BUILD_DIR)/fan-timeline-sol

.PHONY: all sol clean dirs

all: dirs $(TARGET)
sol: dirs $(SOL_TARGET)

dirs:
	@mkdir -p $(BUILD_DIR)/src

$(TARGET): $(BUILD_DIR)/src/main.o $(BUILD_DIR)/src/decide.o | dirs
	$(CC) $(LDFLAGS) -o $@ $^

$(SOL_TARGET): $(BUILD_DIR)/src/main.o $(BUILD_DIR)/src/decide-sol.o | dirs
	$(CC) $(LDFLAGS) -o $@ $^

$(BUILD_DIR)/src/main.o: src/main.c include/fan.h | dirs
	$(CC) $(CFLAGS) -c -o $@ $<

$(BUILD_DIR)/src/decide.o: src/decide.c include/fan.h | dirs
	$(CC) $(CFLAGS) -c -o $@ $<

$(BUILD_DIR)/src/decide-sol.o: src/decide-sol.c include/fan.h | dirs
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -rf $(BUILD_DIR)
