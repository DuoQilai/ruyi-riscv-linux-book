# 学生版：make          → mqtt-led
# 参考实现：make sol    → mqtt-led-sol
# 主机交叉：make CROSS_COMPILE=riscv64-ruyisdk-linux-gnu-
CROSS_COMPILE ?=
CC      := $(CROSS_COMPILE)gcc
CFLAGS  := -Wall -Wextra -O2
LDFLAGS := -lmosquitto -lgpiod
TARGET  := mqtt-led
SOL_TARGET := mqtt-led-sol
SRC     := main.c
SOL_SRC := main-sol.c

.PHONY: all sol clean

all: $(TARGET)

sol: $(SOL_TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(SOL_TARGET): $(SOL_SRC)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -f $(TARGET) $(SOL_TARGET)
