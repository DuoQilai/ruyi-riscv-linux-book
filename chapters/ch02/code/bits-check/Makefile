# bits-check — 第二章实验一：位运算与字节拼装
#
# 学生半成品：  make          → build/bits-check
# 参考实现：    make sol      → build/bits-check-sol
# 在哪台机器 make 就在哪台跑：板端(riscv64)/主机(x86) 的 gcc 都可以。
#
#   make && ./build/bits-check        # 学生版，TODO 未写前会 FAIL
#   make sol && ./build/bits-check-sol  # 参考，应 ALL PASS

CROSS_COMPILE ?=
CC      := $(CROSS_COMPILE)gcc
CFLAGS  := -Wall -Wextra -O0 -g -Iinclude
LDFLAGS :=

BUILD_DIR   := build
TARGET      := $(BUILD_DIR)/bits-check
SOL_TARGET  := $(BUILD_DIR)/bits-check-sol

.PHONY: all sol clean dirs

all: dirs $(TARGET)
sol: dirs $(SOL_TARGET)

dirs:
	@mkdir -p $(BUILD_DIR)/src

$(TARGET): $(BUILD_DIR)/src/main.o $(BUILD_DIR)/src/bits.o | dirs
	$(CC) $(LDFLAGS) -o $@ $^

$(SOL_TARGET): $(BUILD_DIR)/src/main.o $(BUILD_DIR)/src/bits-sol.o | dirs
	$(CC) $(LDFLAGS) -o $@ $^

$(BUILD_DIR)/src/main.o: src/main.c include/bits.h | dirs
	$(CC) $(CFLAGS) -c -o $@ $<

$(BUILD_DIR)/src/bits.o: src/bits.c include/bits.h | dirs
	$(CC) $(CFLAGS) -c -o $@ $<

$(BUILD_DIR)/src/bits-sol.o: src/bits-sol.c include/bits.h | dirs
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -rf $(BUILD_DIR)
