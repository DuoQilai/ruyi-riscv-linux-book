# temperature_fan — 第三章：模拟温度控制继电器/风扇
#
# 默认编学生版：make
# 参考实现：    make sol
#
# 在 x86 Linux 上（需先激活 venv-gnu-ruyisdk）：
#   make / make sol
#   file temperature_fan-sol    # 应含 RISC-V
#   再将可执行文件拷到荔枝派运行
#
# 在荔枝派（riscv64）上：
#   make / make sol
#   sudo ./temperature_fan-sol
#
# 板上目录约定：~/ruyi-riscv-embedded/ch03-lab/

UNAME_M := $(shell uname -m)
ifeq ($(UNAME_M),riscv64)
  CROSS_COMPILE ?=
else
  CROSS_COMPILE ?= riscv64-ruyisdk-linux-gnu-
endif

CC     := $(CROSS_COMPILE)gcc
CFLAGS ?= -Wall -Wextra -O2
LDLIBS ?= -lgpiod

TARGET     := temperature_fan
SRC        := temperature_fan.c
SOL_TARGET := temperature_fan-sol
SOL_SRC    := temperature_fan-sol.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $@ $< $(LDLIBS)

sol: $(SOL_TARGET)

$(SOL_TARGET): $(SOL_SRC)
	$(CC) $(CFLAGS) -o $@ $< $(LDLIBS)

clean:
	rm -f $(TARGET) $(SOL_TARGET)

.PHONY: all sol clean
