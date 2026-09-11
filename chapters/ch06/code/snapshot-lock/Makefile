# snapshot-lock — 第六章实验二：成对快照 + 互斥锁（纯软件）
CROSS_COMPILE ?=
CC      := $(CROSS_COMPILE)gcc
CFLAGS  := -Wall -Wextra -O0 -g -Iinclude -pthread
LDFLAGS := -pthread
BUILD_DIR   := build
TARGET      := $(BUILD_DIR)/snapshot-lock
SOL_TARGET  := $(BUILD_DIR)/snapshot-lock-sol

.PHONY: all sol clean dirs
all: dirs $(TARGET)
sol: dirs $(SOL_TARGET)
dirs:
	@mkdir -p $(BUILD_DIR)/src
$(TARGET): $(BUILD_DIR)/src/main.o $(BUILD_DIR)/src/pair.o | dirs
	$(CC) -o $@ $^ $(LDFLAGS)
$(SOL_TARGET): $(BUILD_DIR)/src/main.o $(BUILD_DIR)/src/pair-sol.o | dirs
	$(CC) -o $@ $^ $(LDFLAGS)
$(BUILD_DIR)/src/%.o: src/%.c include/snapshot.h | dirs
	$(CC) $(CFLAGS) -c -o $@ $<
clean:
	rm -rf $(BUILD_DIR)
