/*
 * main.c — led-cmd-parse 测试驱动（实验一）
 * make && ./build/led-cmd-parse；全绿打印 ALL PASS。别改本文件。
 */
#include <stdio.h>
#include <string.h>
#include "led_cmd.h"

static int fails;

static void expect_parse(const char *in, int want)
{
	int got = parse_led_cmd(in);
	if (got != want) {
		printf("  FAIL parse(\"%s\") got=%d want=%d\n",
		       in ? in : "(null)", got, want);
		fails++;
	}
}

static void expect_status(int on, const char *want)
{
	const char *got = status_payload(on);
	if (!got || strcmp(got, want) != 0) {
		printf("  FAIL status(%d) got=\"%s\" want=\"%s\"\n",
		       on, got ? got : "(null)", want);
		fails++;
	}
}

int main(void)
{
	fails = 0;
	expect_parse("on", 1);
	expect_parse("off", 0);
	expect_parse("ON", -1);
	expect_parse("on ", -1);
	expect_parse("on\n", -1);
	expect_parse("toggle", -1);
	expect_parse("", -1);
	expect_status(1, "on");
	expect_status(0, "off");
	expect_status(42, "on");

	if (fails == 0) {
		printf("  ok   parse on/off 精确匹配\n");
		printf("  ok   status_payload 返回 on/off\n");
		printf("\nALL PASS\n");
		return 0;
	}
	printf("\nSOME FAIL (%d)\n", fails);
	return 1;
}
