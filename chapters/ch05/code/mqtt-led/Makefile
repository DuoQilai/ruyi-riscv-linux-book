# 默认板端原生编译：make
# 主机交叉编译：make CROSS_COMPILE=riscv64-unknown-linux-gnu-
CROSS_COMPILE ?=
CC      := $(CROSS_COMPILE)gcc
CFLAGS  := -Wall -Wextra -O2
LDFLAGS := -lmosquitto -lgpiod
TARGET  := mqtt-led
SRC     := main.c

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -f $(TARGET)
