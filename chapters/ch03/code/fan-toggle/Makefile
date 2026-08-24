# fan-toggle — 第三章完整示例：gpiod 生命周期
#   open chip → 申请 line 5 为输出(初值0) → 拉高 → 拉低 → 释放
#
# 在荔枝派（riscv64）上：
#   make && sudo ./fan-toggle
# 在 x86 Linux 上（先激活 venv-gnu-ruyisdk）：
#   make && file fan-toggle    # 应含 RISC-V

UNAME_M := $(shell uname -m)
ifeq ($(UNAME_M),riscv64)
  CROSS_COMPILE ?=
else
  CROSS_COMPILE ?= riscv64-ruyisdk-linux-gnu-
endif

CC     := $(CROSS_COMPILE)gcc
CFLAGS ?= -Wall -Wextra -O2
LDLIBS ?= -lgpiod

fan-toggle: fan-toggle.c
	$(CC) $(CFLAGS) fan-toggle.c -o $@ $(LDLIBS)

clean:
	rm -f fan-toggle

.PHONY: clean
