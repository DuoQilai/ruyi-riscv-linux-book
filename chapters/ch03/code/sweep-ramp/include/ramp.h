/*
 * ramp.h — 温度扫描边界（实验一素材）
 * 第三章主程序用「24 → 30 → 24」来回扫一个模拟温度。最容易写错的是：
 * 温度越过 30 或 24 还在往前冲（扫描越界）。本实验把这段逻辑抽出来单独练。
 */
#ifndef RAMP_H
#define RAMP_H

#define RAMP_STEP 0.5f

/*
 * 从当前温度 t、方向 dir（+1 升温 / -1 降温）走一步，回到 [lo, hi] 内。
 * 把「下一步」方向写进 *next_dir，返回新的温度：
 *   dir > 0 且 t + RAMP_STEP > hi → 返回 hi，方向改 -1
 *   dir < 0 且 t - RAMP_STEP < lo → 返回 lo，方向改 +1
 *   否则：返回 t + dir*RAMP_STEP，方向不变
 */
float ramp_next(float t, int dir, float lo, float hi, int *next_dir);

#endif
