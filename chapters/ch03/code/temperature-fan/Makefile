# temperature_fan — 第三章 GPIO 输出：假温度阈值控制继电器/风扇
#
# 默认板端原生编译（板子自带 gcc）：make
# 主机交叉编译（需要 riscv64 交叉工具链）：make CROSS_COMPILE=riscv64-unknown-linux-gnu-
#
# 全程 C 语言，不用 C++。

CROSS_COMPILE ?=
CC            ?= $(CROSS_COMPILE)gcc
CFLAGS        ?= -Wall -Wextra -O2
LDLIBS        ?= -lgpiod

TARGET        := temperature_fan
SRC           := temperature_fan.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $@ $< $(LDLIBS)

clean:
	rm -f $(TARGET)

.PHONY: all clean
