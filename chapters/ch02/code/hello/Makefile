# 讲义 2.2 跟做：单文件示例。默认交叉前缀；本机练习用 make CROSS_COMPILE=
CROSS_COMPILE ?= riscv64-unknown-linux-gnu-
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
