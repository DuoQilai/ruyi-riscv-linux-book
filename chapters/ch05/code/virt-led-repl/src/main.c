/*
 * main.c — virt-led-repl 驱动（实验二）。别改本文件。
 * 读 stdin 每行一条命令；quit 退出。
 */
#include <stdio.h>
#include <string.h>
#include "virt_led.h"

int main(void)
{
	char line[128];
	int led_on = 0;

	printf("[repl] on | off | quit   (virtual LED, no GPIO)\n");
	fflush(stdout);
	while (fgets(line, sizeof(line), stdin)) {
		size_t n = strlen(line);
		while (n > 0 && (line[n - 1] == '\n' || line[n - 1] == '\r'))
			line[--n] = '\0';
		if (strcmp(line, "quit") == 0) {
			printf("bye led=%s\n", led_on ? "ON" : "OFF");
			fflush(stdout);
			return 0;
		}
		(void)apply_cmd(line, &led_on);
	}
	return 0;
}
