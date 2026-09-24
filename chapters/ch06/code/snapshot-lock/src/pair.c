/*
 * pair.c — 学生版。对照：make sol
 * 实验三 tri-thread 的 temp_a/temp_b 成对读写缩微版。
 */
#include <stdio.h>
#include <unistd.h>

#include "snapshot.h"

static int g_temp_a;
static int g_temp_b;
/* 实验故意留空窗；加锁后应删掉中间的 usleep 或整段放进同一临界区 */
#define RACE_WINDOW_US 80000

void pair_init(void)
{
	g_temp_a = 0;
	g_temp_b = 0;
}

void pair_destroy(void)
{
}

/* STUDENT TODO：用 pthread_mutex 保护成对写；无锁时中间 sleep 会必现 [RACE] */
void pair_write(int v)
{
	g_temp_a = v;
	usleep(RACE_WINDOW_US); /* 故意空窗——验收加锁后应与 pair_read 同锁 */
	g_temp_b = v;
}

/* STUDENT TODO：同一把锁内读出 a、b */
int pair_read(int *a, int *b)
{
	*a = g_temp_a;
	*b = g_temp_b;
	return (*a != *b) ? 1 : 0;
}
