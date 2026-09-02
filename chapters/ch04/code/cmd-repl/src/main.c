/*
 * main.c — cmd-repl 驱动（实验二）
 * 主循环已给：读一行 → 切首词 → 分发给 status/set/quit。别改本文件，
 * 只实现 handlers.c 里的 state_init / cmd_status / cmd_set。
 *
 * make && ./build/cmd-repl
 * 喂命令测试：printf 'status\nset temp 28.5\nstatus\nquit\n' | ./build/cmd-repl
 */
#include <stdio.h>
#include <string.h>
#include "shell.h"

int main(void)
{
	St   st;
	char buf[128];

	state_init(&st);
	printf("[repl] status | set temp <num> | set humidity <num> | quit\n");

	while (fgets(buf, sizeof(buf), stdin)) {
		char *p, *sp, *args;
		char *nl = strchr(buf, '\n');
		if (nl)
			*nl = '\0';

		p = buf;
		while (*p == ' ' || *p == '\t')
			p++;
		if (!*p)
			continue;

		sp = p;
		while (*sp && *sp != ' ' && *sp != '\t')
			sp++;
		args = "";
		if (*sp) {
			*sp = '\0';
			args = sp + 1;
			while (*args == ' ' || *args == '\t')
				args++;
		}

		if (strcmp(p, "quit") == 0) {
			printf("bye\n");
			break;
		} else if (strcmp(p, "status") == 0) {
			cmd_status(&st);
		} else if (strcmp(p, "set") == 0) {
			cmd_set(&st, args);
		} else {
			printf("unknown: %s\n", p);
		}
	}
	return 0;
}
