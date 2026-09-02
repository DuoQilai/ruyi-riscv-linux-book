/*
 * main.c — sweep-ramp 测试驱动（实验一）
 * make && ./build/sweep-ramp；全绿打印 ALL PASS。别改本文件。
 * 判据：扫 200 步，温度从不越出 [24, 30]；且确实到达过 30 与 24 两个端点。
 */
#include <stdio.h>
#include <math.h>
#include "ramp.h"

#define LO 24.0f
#define HI 30.0f

int main(void)
{
	float t = LO;
	int   dir = 1;
	int   bad = 0, hi_hit = 0, lo_hit = 0, i, shown = 0;

	for (i = 0; i < 200; i++) {
		int nd;
		t = ramp_next(t, dir, LO, HI, &nd);
		dir = nd;

		if (t > HI + 0.0001f || t < LO - 0.0001f) {
			bad++;
			if (shown++ < 3)
				printf("  overshoot step %d: t=%.3f\n", i + 1, t);
		}
		if (fabsf(t - HI) < 0.0001f) hi_hit = 1;
		if (fabsf(t - LO) < 0.0001f) lo_hit = 1;
	}

	if (bad)
		printf("  FAIL 有 %d 步越出 [24, 30]\n", bad);
	else
		printf("  ok   200 步都在 [24, 30] 内\n");
	if (hi_hit)
		printf("  ok   到达过上限 30\n");
	else
		printf("  FAIL 从未到达上限 30\n");
	if (lo_hit)
		printf("  ok   到达过下限 24\n");
	else
		printf("  FAIL 从未到达下限 24\n");

	if (!bad && hi_hit && lo_hit) {
		printf("\nALL PASS\n");
		return 0;
	}
	printf("\nSOME FAIL\n");
	return 1;
}
