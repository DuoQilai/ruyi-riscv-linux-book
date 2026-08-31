# 讲义 2.4 跟做：切词 + 函数指针命令表。本章本机 gcc。
# 交叉时先激活 venv-gnu-ruyisdk，再 make CROSS_COMPILE=riscv64-ruyisdk-linux-gnu-
CROSS_COMPILE ?=
CC      := $(CROSS_COMPILE)gcc
CFLAGS  := -Wall -Wextra -O2
TARGET  := cmd-table
SRC     := main.c

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(TARGET)
