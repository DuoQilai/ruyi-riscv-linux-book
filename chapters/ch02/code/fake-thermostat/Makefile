# fake-thermostat — 第二章闯关脚手架
#
# 默认本机 gcc，专注改 C。可选：
#   make CROSS_COMPILE=riscv64-unknown-linux-gnu-

CROSS_COMPILE ?=
CC      := $(CROSS_COMPILE)gcc
CFLAGS  := -Wall -Wextra -O0 -g -Iinclude
LDFLAGS :=

BUILD_DIR := build
TARGET    := $(BUILD_DIR)/fake-thermo

SRCS := src/main.c src/thermo.c
OBJS := $(patsubst %.c,$(BUILD_DIR)/%.o,$(SRCS))

.PHONY: all clean dirs

all: dirs $(TARGET)

dirs:
	@mkdir -p $(BUILD_DIR)/src

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

$(BUILD_DIR)/%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -rf $(BUILD_DIR)
