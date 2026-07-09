/*
 * ch03 uart-echo — 串口回显（讲义 3.2）
 * 默认打开 UART_DEV；若课堂无第二路串口，可改用 stdin/stdout 模式（见 USE_STDIO）。
 */
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

/* 1 = 用终端 stdin/stdout 练逻辑；0 = 打开真实串口设备 */
#define USE_STDIO 1

#define UART_DEV "/dev/ttyS1" /* TODO: 按板卡修改；USE_STDIO=0 时生效 */
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
	fprintf(stderr, "[INFO] echoing on %s\n", UART_DEV);
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
