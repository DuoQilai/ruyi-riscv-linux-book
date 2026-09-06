# 本机 gcc（非交叉）。用于 ThreadSanitizer。
CC      ?= gcc
CFLAGS  := -Wall -Wextra -O2 -pthread
LDFLAGS := -pthread
TARGET  := race-demo
SRC     := main.c

.PHONY: all clean tsan

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# 打开 ThreadSanitizer：跑 ./race-demo 会打印数据竞争报告
tsan: clean
	$(CC) -Wall -Wextra -O1 -pthread -fsanitize=thread -g -o $(TARGET) $(SRC) $(LDFLAGS)

clean:
	rm -f $(TARGET)
