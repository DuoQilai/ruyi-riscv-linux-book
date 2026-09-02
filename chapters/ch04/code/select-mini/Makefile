# select-mini — 第四章实验一：select 最小多路复用
#
# 学生半成品：  make          → build/select-mini
# 参考实现：    make sol      → build/select-mini-sol
# 本机或板端（riscv64 gcc）均可。
#
#   make && ./build/select-mini
#   自动演示：(sleep 3; echo hello; sleep 1; echo quit) | ./build/select-mini

CROSS_COMPILE ?=
CC      := $(CROSS_COMPILE)gcc
CFLAGS  := -Wall -Wextra -O0 -g -Iinclude
LDFLAGS :=

BUILD_DIR   := build
TARGET      := $(BUILD_DIR)/select-mini
SOL_TARGET  := $(BUILD_DIR)/select-mini-sol

.PHONY: all sol clean dirs

all: dirs $(TARGET)
sol: dirs $(SOL_TARGET)

dirs:
	@mkdir -p $(BUILD_DIR)/src

$(TARGET): $(BUILD_DIR)/src/main.o $(BUILD_DIR)/src/select.o | dirs
	$(CC) $(LDFLAGS) -o $@ $^

$(SOL_TARGET): $(BUILD_DIR)/src/main.o $(BUILD_DIR)/src/select-sol.o | dirs
	$(CC) $(LDFLAGS) -o $@ $^

$(BUILD_DIR)/src/main.o: src/main.c include/select.h | dirs
	$(CC) $(CFLAGS) -c -o $@ $<

$(BUILD_DIR)/src/select.o: src/select.c include/select.h | dirs
	$(CC) $(CFLAGS) -c -o $@ $<

$(BUILD_DIR)/src/select-sol.o: src/select-sol.c include/select.h | dirs
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -rf $(BUILD_DIR)
