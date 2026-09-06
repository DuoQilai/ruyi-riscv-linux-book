#ifndef VIRT_LED_H
#define VIRT_LED_H

/* 处理一行命令（不含换行）。
 * "on"  → 虚拟灯开，打印两行：[LED] ON 与 [STATUS] on，返回 0
 * "off" → 同理 OFF / off，返回 0
 * 其他  → 打印 [ERR] bad cmd，返回 -1
 * 成功改灯时必须先改灯再发 status（与实验三约定一致）。 */
int apply_cmd(const char *line, int *led_on);

#endif
