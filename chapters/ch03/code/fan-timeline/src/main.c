/*
 * main.c — fan-timeline 驱动（实验二）
 * 扫 240 步模拟温度（24 → 30 → 24，走 0.5），每步调 fan_decide，
 * 打印时间线并统计。别改本文件；只实现 src/decide.c。
 *
 * make && ./build/fan-timeline
 * make sol && ./build/fan-timeline-sol   # 参考输出抄进 lab 验收
 */
#include <stdio.h>
#include "fan.h"

#define T_SWEEP_MIN 24.0f
#define T_SWEEP_MAX 30.0f
#define STEP_DELTA  0.5f
#define N_STEPS     240

int main(void)
{
	float t = 24.0f;
	int   dir = 1;
	int   fan = 0, prev;
	int   on_steps = 0, turn_ons = 0, step;

	for (step = 0; step < N_STEPS; step++) {
		/* 到边界就反向：模拟温度保持在 [24, 30] 内（实验一练的就是这段） */
		if (dir > 0 && t + STEP_DELTA > T_SWEEP_MAX) {
			t = T_SWEEP_MAX;
			dir = -1;
		} else if (dir < 0 && t - STEP_DELTA < T_SWEEP_MIN) {
			t = T_SWEEP_MIN;
			dir = 1;
		} else {
			t += dir * STEP_DELTA;
		}

		prev = fan;
		fan_decide(t, &fan);
		if (fan && !prev)
			turn_ons++;          /* 0→1 一次算一次「开风扇」动作 */
		if (fan)
			on_steps++;

		printf("[%03d] t=%6.2f fan=%s\n", step, t, fan ? "ON" : "OFF");
	}

	printf("SUMMARY steps=%d on_steps=%d turn_ons=%d\n",
	       N_STEPS, on_steps, turn_ons);
	return 0;
}
