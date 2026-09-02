# cmd-parse — 第二章实验二：切词与数值解析
#
# 学生半成品：  make          → build/cmd-parse
# 参考实现：    make sol      → build/cmd-parse-sol
# 在哪台机器 make 就在哪台跑：板端(riscv64)/主机(x86) 的 gcc 都可以。
#
#   make && ./build/cmd-parse
#   make sol && ./build/cmd-parse-sol

CROSS_COMPILE ?=
CC      := $(CROSS_COMPILE)gcc
CFLAGS  := -Wall -Wextra -O0 -g -Iinclude -lm
LDFLAGS := -lm

BUILD_DIR   := build
TARGET      := $(BUILD_DIR)/cmd-parse
SOL_TARGET  := $(BUILD_DIR)/cmd-parse-sol

.PHONY: all sol clean dirs

all: dirs $(TARGET)
sol: dirs $(SOL_TARGET)

dirs:
	@mkdir -p $(BUILD_DIR)/src

$(TARGET): $(BUILD_DIR)/src/main.o $(BUILD_DIR)/src/parse.o | dirs
	$(CC) $(LDFLAGS) -o $@ $^

$(SOL_TARGET): $(BUILD_DIR)/src/main.o $(BUILD_DIR)/src/parse-sol.o | dirs
	$(CC) $(LDFLAGS) -o $@ $^

$(BUILD_DIR)/src/main.o: src/main.c include/parse.h | dirs
	$(CC) $(CFLAGS) -c -o $@ $<

$(BUILD_DIR)/src/parse.o: src/parse.c include/parse.h | dirs
	$(CC) $(CFLAGS) -c -o $@ $<

$(BUILD_DIR)/src/parse-sol.o: src/parse-sol.c include/parse.h | dirs
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -rf $(BUILD_DIR)
