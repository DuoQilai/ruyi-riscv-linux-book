/*
 * ramp-sol.c — sweep-ramp 参考实现（make sol）
 */
#include "ramp.h"

float ramp_next(float t, int dir, float lo, float hi, int *next_dir)
{
	if (dir > 0 && t + RAMP_STEP > hi) {
		*next_dir = -1;
		return hi;
	}
	if (dir < 0 && t - RAMP_STEP < lo) {
		*next_dir = 1;
		return lo;
	}
	*next_dir = dir;
	return t + dir * RAMP_STEP;
}
