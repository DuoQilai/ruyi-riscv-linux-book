# CoreMark — 第一章：交叉编译 / 板上原生编译（同一份 Makefile）
#
# 在 x86 Linux 上（需先激活 venv-gnu-ruyisdk）：
#   . ~/venv-gnu-ruyisdk/bin/ruyi-activate   # 路径以你创建位置为准
#   make
#   file coremark/coremark.exe               # 应含 RISC-V
#
# 在荔枝派（riscv64）上：
#   make
#   ./coremark/coremark.exe
#
# 板上同步目录约定：~/ruyi-riscv-embedded/ch01-lab/
# 本章无学生挖空版：CoreMark 用于工具链与上板跑分验证。

UNAME_M := $(shell uname -m)
ifeq ($(UNAME_M),riscv64)
  CROSS_COMPILE ?=
else
  CROSS_COMPILE ?= riscv64-ruyisdk-linux-gnu-
endif

CC       := $(CROSS_COMPILE)gcc
PORT_DIR := posix

.PHONY: all clean

# 只编译链接，不在主机上执行（交叉产物无法在 x86 上跑）
all:
	$(MAKE) -C coremark \
		PORT_DIR=$(PORT_DIR) \
		CC=$(CC) \
		LD=$(CC) \
		compile link

clean:
	$(MAKE) -C coremark PORT_DIR=$(PORT_DIR) clean
	rm -f coremark/coremark.exe coremark/*.o
