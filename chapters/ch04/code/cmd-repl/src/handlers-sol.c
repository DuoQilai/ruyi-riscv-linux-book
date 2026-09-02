/*
 * handlers-sol.c — cmd-repl 参考实现（make sol）
 * 这套 status/set 语义就是实验三 cmd_thermo-sol 里 cmd_status/cmd_set 的骨架。
 */
#include <stdio.h>
#include <string.h>
#include "shell.h"

void state_init(St *s)
{
	if (!s)
		return;
	s->t_bar = 30.0f;
	s->h_bar = 95.0f;
}

int cmd_status(const St *s)
{
	if (!s)
		return -1;
	printf("[status] temp_bar=%.1fC hum_bar=%.1f%%\n", s->t_bar, s->h_bar);
	fflush(stdout);
	return 0;
}

int cmd_set(St *s, const char *args)
{
	char what[16];
	float v;

	if (!s || !args)
		return -1;
	if (sscanf(args, "%15s %f", what, &v) != 2) {
		printf("[err] usage: set temp <num> | set humidity <num>\n");
		fflush(stdout);
		return -1;
	}
	if (strcmp(what, "temp") == 0 || strcmp(what, "temperature") == 0) {
		s->t_bar = v;
		printf("[ok] temp_bar=%.1fC\n", s->t_bar);
	} else if (strcmp(what, "humidity") == 0 || strcmp(what, "hum") == 0) {
		s->h_bar = v;
		printf("[ok] hum_bar=%.1f%%\n", s->h_bar);
	} else {
		printf("[err] unknown field: %s (use temp or humidity)\n", what);
		fflush(stdout);
		return -1;
	}
	fflush(stdout);
	return 0;
}
