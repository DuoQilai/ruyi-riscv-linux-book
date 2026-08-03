# 默认板端原生编译（板子自带 gcc）：make
# 主机交叉编译：make CROSS_COMPILE=riscv64-unknown-linux-gnu-
CROSS_COMPILE ?=
CC      := $(CROSS_COMPILE)gcc
CFLAGS  := -Wall -Wextra -O2
LDFLAGS := -lgpiod
TARGET  := serial-thermo
SRC     := main.c

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -f $(TARGET)
