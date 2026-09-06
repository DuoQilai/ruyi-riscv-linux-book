/*
 * virt_led.c — 学生版。对照：make sol
 * 实验三 on_message 里「解析 → 改灯 → 发 status」的纯软件版。
 */
#include <stdio.h>
#include <string.h>
#include "virt_led.h"

/* STUDENT TODO */
int apply_cmd(const char *line, int *led_on)
{
	(void)line;
	(void)led_on;
	printf("[TODO] apply_cmd not implemented\n");
	fflush(stdout);
	return -1;
}
