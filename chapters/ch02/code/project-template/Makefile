# 讲义 2.2 跟做：多文件工程。本章本机 gcc。
# 交叉时先激活 venv-gnu-ruyisdk，再 make CROSS_COMPILE=riscv64-ruyisdk-linux-gnu-
CROSS_COMPILE ?=
CC      := $(CROSS_COMPILE)gcc
CFLAGS  := -Wall -Wextra -O2 -Iinclude
LDFLAGS :=

BUILD_DIR := build
TARGET    := $(BUILD_DIR)/app

SRCS := src/main.c src/greet.c
OBJS := $(patsubst %.c,$(BUILD_DIR)/%.o,$(SRCS))

.PHONY: all clean dirs

all: dirs $(TARGET)

dirs:
	@mkdir -p $(BUILD_DIR)/src

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

$(BUILD_DIR)/%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -rf $(BUILD_DIR)
