/*
 * fan.h — 滞回决策（实验二素材）
 * 阈值与第三章主实验 temperature_fan 完全一致：
 *   > 29°C 开，< 25°C 关，中间保持。
 */
#ifndef FAN_H
#define FAN_H

#define T_HIGH 29.0f
#define T_LOW  25.0f

/*
 * 根据当前温度 temp_c 更新 *fan_on（0=OFF, 1=ON）。
 *   temp_c > T_HIGH → *fan_on = 1
 *   temp_c < T_LOW  → *fan_on = 0
 *   中间区间         → 保持 *fan_on
 * fan_on 为 NULL 时直接返回。
 * 注意：签名与实验三 temperature_fan 的 fan_decide 一致，
 * 本实验写好后可直接平移到实验三，只需把「执行」换成真 GPIO。
 */
void fan_decide(float temp_c, int *fan_on);

#endif
