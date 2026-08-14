/*
 * 讲义 2.4 跟做：按空格切词 + 函数指针命令表。
 * 这就是第四章 cmd_thermo 命令表的缩小版（无 GPIO、无 select）。
 *
 *   make
 *   printf 'help\nstatus\nset temp 20\nstatus\nfoo\n' | ./cmd-table
 */
#include <stdio.h>
#include <string.h>

static float g_t_bar = 28.0f;

static int cmd_help(char *args)
{
	(void)args;
	printf("commands: help status set\n");
	return 0;
}

static int cmd_status(char *args)
{
	(void)args;
	printf("t_bar=%.1f\n", g_t_bar);
	return 0;
}

static int cmd_set(char *args)
{
	char what[16];
	float v;

	if (!args || sscanf(args, "%15s %f", what, &v) != 2 ||
	    strcmp(what, "temp") != 0) {
		printf("usage: set temp <n>\n");
		return -1;
	}
	g_t_bar = v;
	printf("t_bar=%.1f\n", g_t_bar);
	return 0;
}

struct cmd_entry {
	const char *name;
	int (*handler)(char *args);
};

static const struct cmd_entry g_cmds[] = {
	{ "help",   cmd_help },
	{ "status", cmd_status },
	{ "set",    cmd_set },
	{ NULL,     NULL },
};

static int dispatch(char *line)
{
	char *nl, *sp, *args;
	const struct cmd_entry *e;

	nl = strchr(line, '\n');
	if (nl)
		*nl = '\0';

	sp = strchr(line, ' ');
	if (sp) {
		*sp = '\0';
		args = sp + 1;
		while (*args == ' ')
			args++;
	} else {
		args = line + strlen(line);
	}

	if (line[0] == '\0')
		return 0;

	for (e = g_cmds; e->name; e++) {
		if (strcmp(line, e->name) == 0)
			return e->handler(args);
	}
	printf("unknown command: %s\n", line);
	return -1;
}

int main(void)
{
	char buf[128];

	while (fgets(buf, sizeof(buf), stdin))
		dispatch(buf);
	return 0;
}
