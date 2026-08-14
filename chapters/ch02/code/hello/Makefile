# 讲义 2.2 跟做：单文件示例。本章本机 gcc。
# 交叉时先激活 venv-gnu-ruyisdk，再 make CROSS_COMPILE=riscv64-ruyisdk-linux-gnu-
CROSS_COMPILE ?=
CC      := $(CROSS_COMPILE)gcc
CFLAGS  := -Wall -Wextra -O2
TARGET  := hello
SRC     := main.c

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(TARGET)
