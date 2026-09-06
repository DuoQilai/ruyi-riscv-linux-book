/* led_cmd.h — 第五章实验一：解析 on/off，生成 status 载荷 */
#ifndef LED_CMD_H
#define LED_CMD_H

/* 精确匹配 "on" → 返回 1；"off" → 返回 0；其他 → 返回 -1。
 * 不允许前后空格变体，不允许 "ON"/"On"。 */
int parse_led_cmd(const char *payload);

/* on!=0 → 返回 "on"；否则返回 "off"。返回的是静态字符串，勿 free。 */
const char *status_payload(int on);

#endif
