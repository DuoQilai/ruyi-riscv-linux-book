/*
 * thermo.c — 第二章闯关半成品（请按 lab 关卡修复）
 *
 * 【学生动手】本文件全部是你要改的区域。参考实现见 thermo-sol.c（make sol）。
 * STUDENT TODO 关卡（一次只改一类）：
 *   L1  比较方向写反
 *   L2  误用 period_ms 当阈值
 *   L3  未检查空指针就解引用
 *   L4  读失败仍返回「成功」并写出垃圾温度
 *
 * 对照：chapters/ch02/lab.html 关卡表；脚手架 README.md
 */
#include "thermo.h"

#include <stddef.h>

/* 由 main 注入：下一笔假读数；fail 非 0 表示本轮读失败 */
static float g_next_temp;
static int   g_force_fail;

void thermo_inject(float temp_c, int fail)
{
	g_next_temp = temp_c;
	g_force_fail = fail;
}

int thermo_read(float *out_c)
{
	/* STUDENT TODO [L3]：out_c 为空时应失败返回，不能解引用 */
	if (g_force_fail) {
		/* STUDENT TODO [L4]：读失败应返回非 0，且不要写 *out_c */
		*out_c = g_next_temp; /* BUG */
		return 0;             /* BUG */
	}

	*out_c = g_next_temp;
	return 0;
}

void thermo_decide(const ThermoConfig *cfg, float temp_c, int *fan_on)
{
	/* STUDENT TODO [L3]：cfg / fan_on 为空时安全返回 */
	/* STUDENT TODO [L1]：比较方向写反（该开却关 / 该关却开） */
	/* STUDENT TODO [L2]：用 period_ms 冒充 t_high —— 应使用 cfg->t_high */
	if (temp_c > (float)cfg->period_ms) {
		*fan_on = 0; /* BUG：该开却关 */
	} else if (temp_c < cfg->t_low) {
		*fan_on = 1; /* BUG：该关却开 */
	}
}
