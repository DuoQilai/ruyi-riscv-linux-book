/*
 * uart-echo — 可选演示：终端/串口回显
 *
 * 【非第三章实验必做】本章实验工程是 temperature-fan/。
 * 本目录仅作「stdin 读写」可选练习；默认 USE_STDIO=1，不必接物理串口。
 *
 * 若改 USE_STDIO=0：把下行 UART_DEV 改成板子上真实设备节点。
 */
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

/* 1 = 用终端 stdin/stdout；0 = 打开真实串口设备（可选） */
#define USE_STDIO 1

#define UART_DEV "/dev/ttyS1" /* 仅 USE_STDIO=0 时需要；按板卡修改 */
#define BAUD     B115200

static int setup_tty(int fd)
{
	struct termios t;

	if (tcgetattr(fd, &t) < 0)
		return -1;
	cfmakeraw(&t);
	cfsetispeed(&t, BAUD);
	cfsetospeed(&t, BAUD);
	t.c_cflag |= (CLOCAL | CREAD);
	t.c_cc[VMIN] = 1;
	t.c_cc[VTIME] = 0;
	if (tcsetattr(fd, TCSANOW, &t) < 0)
		return -1;
	return 0;
}

int main(void)
{
	int fd_in, fd_out;
	char buf[256];
	ssize_t n;

#if USE_STDIO
	fd_in = STDIN_FILENO;
	fd_out = STDOUT_FILENO;
	printf("[INFO] uart-echo in STDIO mode (type lines, Ctrl+D to quit)\n");
#else
	fd_in = open(UART_DEV, O_RDWR | O_NOCTTY);
	if (fd_in < 0) {
		perror("open UART");
		return 1;
	}
	if (setup_tty(fd_in) < 0) {
		perror("setup_tty");
		close(fd_in);
		return 1;
	}
	fd_out = fd_in;
	printf("[INFO] echo on %s\n", UART_DEV);
#endif

	while ((n = read(fd_in, buf, sizeof(buf))) > 0) {
		if (write(fd_out, buf, (size_t)n) != n) {
			perror("write");
			break;
		}
	}
	if (n < 0)
		perror("read");

#if !USE_STDIO
	close(fd_in);
#endif
	return 0;
}
