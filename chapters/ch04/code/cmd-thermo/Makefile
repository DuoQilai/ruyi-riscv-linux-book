# cmd-thermo — 第四章：SSH 终端命令 + 温湿度控风扇
#
# 默认编学生版：make
# 参考实现：    make sol

CROSS_COMPILE ?=
CC            ?= $(CROSS_COMPILE)gcc
CFLAGS        ?= -Wall -Wextra -O2
LDLIBS        ?= -lgpiod

TARGET        := cmd-thermo
SRC           := main.c

SOL_TARGET    := cmd-thermo-sol
SOL_SRC       := main-sol.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $@ $< $(LDLIBS)

sol: $(SOL_TARGET)

$(SOL_TARGET): $(SOL_SRC)
	$(CC) $(CFLAGS) -o $@ $< $(LDLIBS)

clean:
	rm -f $(TARGET) $(SOL_TARGET)

.PHONY: all sol clean
