/*
 * led_cmd.c — 学生版。对照：make sol
 */
#include "led_cmd.h"

/* STUDENT TODO：精确匹配 on/off。 */
int parse_led_cmd(const char *payload)
{
	(void)payload;
	return -1; /* ← 这里实现 */
}

/* STUDENT TODO：返回 "on" 或 "off"。 */
const char *status_payload(int on)
{
	(void)on;
	return ""; /* ← 这里实现 */
}
