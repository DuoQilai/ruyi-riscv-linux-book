/*
 * decide-sol.c — fan-timeline 参考实现（make sol）
 * 这段就是实验三 temperature_fan-sol 里 fan_decide 的本体。
 */
#include "fan.h"

void fan_decide(float temp_c, int *fan_on)
{
	if (!fan_on)
		return;
	if (temp_c > T_HIGH)
		*fan_on = 1;
	else if (temp_c < T_LOW)
		*fan_on = 0;
	/* 中间区间：不改 *fan_on，保持 */
}
