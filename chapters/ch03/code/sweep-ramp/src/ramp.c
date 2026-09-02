/*
 * ramp.c — 温度扫描边界（学生版）
 * 只实现 ramp_next。对照答案：make sol && ./build/sweep-ramp-sol
 */
#include "ramp.h"

/* STUDENT TODO [R1]：按头文件契约写一步扫描，别让温度冲出 [lo, hi]。
 * 到边界前就反向（钳在边界），而不是越过去再反。 */
float ramp_next(float t, int dir, float lo, float hi, int *next_dir)
{
	return t;   /* ← 这里实现（先占位：不动） */
}
