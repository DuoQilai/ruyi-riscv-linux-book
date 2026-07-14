/*
 * thermo.c — 故意有坑的半成品实现（请按 lab 关卡修复）
 *
 * BUG 提示（修完后可删这些注释）：
 *   L1  比较方向写反
 *   L2  误用 period_ms 当阈值
 *   L3  未检查空指针就解引用
 *   L4  读失败仍返回「成功」并写出垃圾温度 —— 见 main 侧配合；
 *       本文件 thermo_read 在 fail 路径也缺正确返回
 */
#include "thermo.h"

#include <stddef.h>

/* 由 main 注入：下一笔假读数；负值表示本轮读失败 */
static float g_next_temp;
static int   g_force_fail;

void thermo_inject(float temp_c, int fail)
{
	g_next_temp = temp_c;
	g_force_fail = fail;
}

int thermo_read(float *out_c)
{
	/* L3 相关：out_c 为空时应失败返回，不能解引用 */
	if (g_force_fail) {
		/* L4：读失败应返回非 0，且不要写 *out_c */
		*out_c = g_next_temp; /* BUG: 失败仍写入并落下去 return 0 */
		return 0;             /* BUG: 应返回非 0 */
	}

	*out_c = g_next_temp;
	return 0;
}

void thermo_decide(const ThermoConfig *cfg, float temp_c, int *fan_on)
{
	/* L3 BUG: 未判 NULL */
	/* L1 BUG: 比较方向写反 */
	/* L2 BUG: 用 period_ms 冒充 t_high */
	if (temp_c > (float)cfg->period_ms) {
		*fan_on = 0; /* 该开却关 */
	} else if (temp_c < cfg->t_low) {
		*fan_on = 1; /* 该关却开 */
	}
}
