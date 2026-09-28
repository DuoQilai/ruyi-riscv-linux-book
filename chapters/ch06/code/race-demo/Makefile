# 主机或板端 gcc。交叉：make CROSS_COMPILE=riscv64-ruyisdk-linux-gnu-
# TSan 用本机/板端原生 gcc（交叉工具链通常不开 TSan）。
CROSS_COMPILE ?=
CC      := $(CROSS_COMPILE)gcc
CFLAGS  := -Wall -Wextra -O2 -pthread
LDFLAGS := -pthread
TARGET  := race-demo
SRC     := main.c

.PHONY: all clean tsan

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

tsan: clean
	$(CC) -Wall -Wextra -O1 -pthread -fsanitize=thread -g -o $(TARGET) $(SRC) $(LDFLAGS)

clean:
	rm -f $(TARGET)
