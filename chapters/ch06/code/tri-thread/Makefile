# 默认板端原生编译：make
# 主机交叉编译：make CROSS_COMPILE=riscv64-ruyisdk-linux-gnu-
# 加锁对照：  make USE_LOCK=1        （默认 0：故意无锁，必现 [RACE]）
CROSS_COMPILE ?=
USE_LOCK ?= 0
CC      := $(CROSS_COMPILE)gcc
CFLAGS  := -Wall -Wextra -O2 -pthread -DUSE_LOCK=$(USE_LOCK)
LDFLAGS := -pthread -lmosquitto -lgpiod
TARGET  := tri-thread
SRC     := main.c

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# USE_LOCK 变了也要重编
.PHONY: rebuild
rebuild: clean all

clean:
	rm -f $(TARGET)
