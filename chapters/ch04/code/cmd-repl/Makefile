# cmd-repl — 第四章实验二：温控命令 REPL
#
# 学生半成品：  make          → build/cmd-repl
# 参考实现：    make sol      → build/cmd-repl-sol
# 本机或板端（riscv64 gcc）均可。
#
#   make && ./build/cmd-repl
#   printf 'status\nset temp 28.5\nstatus\nquit\n' | ./build/cmd-repl

CROSS_COMPILE ?=
CC      := $(CROSS_COMPILE)gcc
CFLAGS  := -Wall -Wextra -O0 -g -Iinclude
LDFLAGS :=

BUILD_DIR   := build
TARGET      := $(BUILD_DIR)/cmd-repl
SOL_TARGET  := $(BUILD_DIR)/cmd-repl-sol

.PHONY: all sol clean dirs

all: dirs $(TARGET)
sol: dirs $(SOL_TARGET)

dirs:
	@mkdir -p $(BUILD_DIR)/src

$(TARGET): $(BUILD_DIR)/src/main.o $(BUILD_DIR)/src/handlers.o | dirs
	$(CC) $(LDFLAGS) -o $@ $^

$(SOL_TARGET): $(BUILD_DIR)/src/main.o $(BUILD_DIR)/src/handlers-sol.o | dirs
	$(CC) $(LDFLAGS) -o $@ $^

$(BUILD_DIR)/src/main.o: src/main.c include/shell.h | dirs
	$(CC) $(CFLAGS) -c -o $@ $<

$(BUILD_DIR)/src/handlers.o: src/handlers.c include/shell.h | dirs
	$(CC) $(CFLAGS) -c -o $@ $<

$(BUILD_DIR)/src/handlers-sol.o: src/handlers-sol.c include/shell.h | dirs
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -rf $(BUILD_DIR)
