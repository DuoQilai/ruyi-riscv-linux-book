# fake-thermostat — 第二章闯关脚手架
#
# 学生半成品：  make          → build/fake-thermo
# 参考实现：    make sol      → build/fake-thermo-sol
#
# 在 x86 Linux 上（本章主路径，本机 gcc）：
#   make clean && make sol && printf '25\n27\n29\n27\n24\n-1\n' | ./build/fake-thermo-sol --stdin
#
# 在荔枝派上（可选复跑，同样用板载 gcc）：
#   make CROSS_COMPILE= sol && ...

CROSS_COMPILE ?=
CC      := $(CROSS_COMPILE)gcc
CFLAGS  := -Wall -Wextra -O0 -g -Iinclude
LDFLAGS :=

BUILD_DIR := build
TARGET    := $(BUILD_DIR)/fake-thermo
SOL_TARGET := $(BUILD_DIR)/fake-thermo-sol

SRCS     := src/main.c src/thermo.c
SOL_SRCS := src/main.c src/thermo-sol.c
OBJS     := $(patsubst %.c,$(BUILD_DIR)/%.o,$(SRCS))
SOL_OBJS := $(patsubst %.c,$(BUILD_DIR)/%-sol.o,$(SOL_SRCS))

.PHONY: all sol clean dirs

all: dirs $(TARGET)

sol: dirs $(SOL_TARGET)

dirs:
	@mkdir -p $(BUILD_DIR)/src

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

$(SOL_TARGET): $(BUILD_DIR)/src/main-sol.o $(BUILD_DIR)/src/thermo-sol.o
	$(CC) $(LDFLAGS) -o $@ $^

$(BUILD_DIR)/src/main.o: src/main.c
	$(CC) $(CFLAGS) -c -o $@ $<

$(BUILD_DIR)/src/thermo.o: src/thermo.c
	$(CC) $(CFLAGS) -c -o $@ $<

$(BUILD_DIR)/src/main-sol.o: src/main.c
	$(CC) $(CFLAGS) -c -o $@ $<

$(BUILD_DIR)/src/thermo-sol.o: src/thermo-sol.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -rf $(BUILD_DIR)
