# probe — 第一章最小程序（实验一 / 实验二素材）
#
# 规则：在哪台机器 make，产物就是哪台机器的架构。
#   荔枝派(riscv64) make → probe 是 RISC-V ELF
#   主机(x86-64)    make → probe 是 x86-64 ELF（不能上板跑！）
#
# 用同一份 Makefile 跑两种机器，正是讲义 1.3「一键编译 / Makefile 双路径」的用意。

CC     ?= gcc
CFLAGS ?= -O2 -Wall -Wextra
TARGET  = probe

all: $(TARGET)

$(TARGET): probe.c
	$(CC) $(CFLAGS) -o $@ probe.c

clean:
	rm -f $(TARGET)
