/*
 * select-sol.c — select-mini 参考实现（make sol）
 */
#include <stdio.h>
#include <string.h>
#include <sys/select.h>
#include <unistd.h>
#include "select.h"

int run_repl_loop(int fd, double tick_s)
{
	char buf[128];
	int  tick = 0;

	printf("[S] select demo: %.0fs tick, stdin echo, quit to exit\n", tick_s);
	fflush(stdout);

	for (;;) {
		fd_set rfds;
		struct timeval tv;
		int r;
		size_t len;

		FD_ZERO(&rfds);
		FD_SET(fd, &rfds);
		tv.tv_sec = (long)tick_s;
		tv.tv_usec = (long)((tick_s - (long)tick_s) * 1e6);

		r = select(fd + 1, &rfds, NULL, NULL, &tv);
		if (r < 0) {
			perror("select");
			return 1;
		}
		if (r == 0) {
			tick++;
			printf("[tick %d]\n", tick);
			fflush(stdout);
			continue;
		}
		if (!FD_ISSET(fd, &rfds))
			continue;

		if (!fgets(buf, sizeof(buf), stdin)) {
			printf("[EOF]\n");
			return 0;
		}
		len = strlen(buf);
		while (len > 0 && (buf[len - 1] == '\n' || buf[len - 1] == '\r'))
			buf[--len] = '\0';
		if (strcmp(buf, "quit") == 0) {
			printf("bye\n");
			return 0;
		}
		printf("echo: %s\n", buf);
		fflush(stdout);
	}
}
