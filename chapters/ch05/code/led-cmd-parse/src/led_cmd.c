/*
 * led_cmd.c — 学生版。对照：make sol
 */
#include "led_cmd.h"

/* STUDENT TODO：按 payloadlen 精确匹配 on/off（用 memcmp，别用 strcmp）。 */
int parse_led_cmd(const char *payload, int payloadlen)
{
	(void)payload;
	(void)payloadlen;
	return -1; /* ← 这里实现 */
}

/* STUDENT TODO：返回 "on" 或 "off"。 */
const char *status_payload(int on)
{
	(void)on;
	return ""; /* ← 这里实现 */
}
