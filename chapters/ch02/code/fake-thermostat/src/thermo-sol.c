/*
 * thermo-sol.c — 第二章参考实现（make sol）
 *
 * 学生动手改 thermo.c。本文件为正确行为，供教师/助教验收。
 */
#include "thermo.h"

#include <stddef.h>

static float g_next_temp;
static int   g_force_fail;

void thermo_inject(float temp_c, int fail)
{
	g_next_temp = temp_c;
	g_force_fail = fail;
}

int thermo_read(float *out_c)
{
	if (out_c == NULL)
		return -1;
	if (g_force_fail)
		return -1;
	*out_c = g_next_temp;
	return 0;
}

void thermo_decide(const ThermoConfig *cfg, float temp_c, int *fan_on)
{
	if (cfg == NULL || fan_on == NULL)
		return;
	if (temp_c > cfg->t_high)
		*fan_on = 1;
	else if (temp_c < cfg->t_low)
		*fan_on = 0;
	/* 中间保持 *fan_on 不变 */
}
