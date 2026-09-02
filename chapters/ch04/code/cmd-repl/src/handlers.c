/*
 * handlers.c — 温控命令处理器（学生版）
 * 只实现 state_init / cmd_status / cmd_set。对照：make sol
 */
#include <stdio.h>
#include <string.h>
#include "shell.h"

/* STUDENT TODO [C1]：初始 t_bar=30.0，h_bar=95.0。 */
void state_init(St *s)
{
	(void)s;   /* ← 这里实现 */
}

/* STUDENT TODO [C2]：打印当前 bar。
 * 例：[status] temp_bar=30.0C hum_bar=95.0%   （打印后记得 fflush） */
int cmd_status(const St *s)
{
	(void)s;
	return 0;   /* ← 这里实现 */
}

/* STUDENT TODO [C3]：解析 args。形如 "temp 28.5" / "humidity 60"。
 * 认 temp|temperature 与 humidity|hum；格式错打一行 [err] 用法并返回非 0。 */
int cmd_set(St *s, const char *args)
{
	(void)s; (void)args;
	return -1;   /* ← 这里实现 */
}
