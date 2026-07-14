/*
 * thermo.h — 假温控配置与传感器 API
 * 讲义 2.2 / 2.3；实验：改代码闯关
 */
#ifndef THERMO_H
#define THERMO_H

typedef struct {
	float t_high;    /* °C：高于此开风扇 */
	float t_low;     /* °C：低于此关风扇（滞回） */
	int   period_ms; /* 采样周期提示，决策逻辑不应当温度阈值用 */
} ThermoConfig;

/*
 * 测试注入：设置下一笔假读数。
 * fail != 0 表示本轮传感器失败（thermo_read 应返回非 0）。
 */
void thermo_inject(float temp_c, int fail);

/* 读一次温度到 *out_c；成功返回 0，失败返回非 0；out_c 为 NULL 时返回非 0 */
int thermo_read(float *out_c);

/*
 * 根据 cfg 与当前温度更新 *fan_on（0=OFF, 1=ON）。
 * 滞回：temp > t_high → ON；temp < t_low → OFF；中间保持。
 * cfg 或 fan_on 为 NULL 时应安全返回。
 */
void thermo_decide(const ThermoConfig *cfg, float temp_c, int *fan_on);

#endif
