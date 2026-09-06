# 学生版：make          → mqtt-led
# 参考实现：make sol    → mqtt-led-sol
# 主机交叉：make CROSS_COMPILE=riscv64-ruyisdk-linux-gnu-
# 解析复用实验一 led-cmd-parse（须先完成实验一）
CROSS_COMPILE ?=
CC      := $(CROSS_COMPILE)gcc
CFLAGS  := -Wall -Wextra -O2 -I../led-cmd-parse/include
LDFLAGS := -lmosquitto -lgpiod
TARGET  := mqtt-led
SOL_TARGET := mqtt-led-sol
LED_CMD_SRC := ../led-cmd-parse/src/led_cmd.c
LED_CMD_SOL := ../led-cmd-parse/src/led_cmd-sol.c

.PHONY: all sol clean

all: $(TARGET)

sol: $(SOL_TARGET)

$(TARGET): main.c $(LED_CMD_SRC)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(SOL_TARGET): main-sol.c $(LED_CMD_SOL)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -f $(TARGET) $(SOL_TARGET)
