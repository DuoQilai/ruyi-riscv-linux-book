# 默认板端原生编译：make
# 主机交叉编译：make CROSS_COMPILE=riscv64-ruyisdk-linux-gnu-
CROSS_COMPILE ?=
CC      := $(CROSS_COMPILE)gcc
CFLAGS  := -Wall -Wextra -O2 -pthread
LDFLAGS := -pthread -lmosquitto -lgpiod
TARGET  := tri-thread
SRC     := main.c

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -f $(TARGET)
