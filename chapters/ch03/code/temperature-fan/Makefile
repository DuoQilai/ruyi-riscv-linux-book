# temperature_fan — 第三章 GPIO 输出：假温度阈值控制继电器/风扇
#
# 默认编学生版：make
# 参考实现：    make sol
# 板端原生（板子自带 gcc）：make
# 主机交叉：    make CROSS_COMPILE=riscv64-unknown-linux-gnu-
#
# 全程 C 语言，不用 C++。

CROSS_COMPILE ?=
CC            ?= $(CROSS_COMPILE)gcc
CFLAGS        ?= -Wall -Wextra -O2
LDLIBS        ?= -lgpiod

TARGET        := temperature_fan
SRC           := temperature_fan.c

SOL_TARGET    := temperature_fan-sol
SOL_SRC       := temperature_fan-sol.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $@ $< $(LDLIBS)

sol: $(SOL_TARGET)

$(SOL_TARGET): $(SOL_SRC)
	$(CC) $(CFLAGS) -o $@ $< $(LDLIBS)

clean:
	rm -f $(TARGET) $(SOL_TARGET)

.PHONY: all sol clean
