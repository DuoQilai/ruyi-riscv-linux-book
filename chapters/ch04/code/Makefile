# cmd_thermo — 第四章：SSH 终端命令 + 温湿度控风扇
#
# 默认编学生版：make
# 参考实现：    make sol
# 强制关风扇：  make fan-off && sudo ./fan_off
#
# 在 x86 Linux 上（需先激活 venv-gnu-ruyisdk）：
#   make sol fan-off
#   file cmd_thermo-sol          # 应含 RISC-V
#   再将可执行文件拷到荔枝派运行
#
# 在荔枝派（riscv64）上：
#   make sol fan-off
#   sudo ./cmd_thermo-sol
#
# 板上目录约定：~/ruyi-riscv-embedded/ch04-lab/

UNAME_M := $(shell uname -m)
ifeq ($(UNAME_M),riscv64)
  CROSS_COMPILE ?=
else
  CROSS_COMPILE ?= riscv64-ruyisdk-linux-gnu-
endif

CC     := $(CROSS_COMPILE)gcc
CFLAGS ?= -Wall -Wextra -O2
LDLIBS ?= -lgpiod

TARGET        := cmd_thermo
SRC           := cmd_thermo.c
SOL_TARGET    := cmd_thermo-sol
SOL_SRC       := cmd_thermo-sol.c
FANOFF_TARGET := fan_off
FANOFF_SRC    := fan_off.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $@ $< $(LDLIBS)

sol: $(SOL_TARGET)

$(SOL_TARGET): $(SOL_SRC)
	$(CC) $(CFLAGS) -o $@ $< $(LDLIBS)

fan-off: $(FANOFF_TARGET)

$(FANOFF_TARGET): $(FANOFF_SRC)
	$(CC) $(CFLAGS) -o $@ $< $(LDLIBS)

clean:
	rm -f $(TARGET) $(SOL_TARGET) $(FANOFF_TARGET)

.PHONY: all sol fan-off clean
