/*
 * main.c — led-cmd-parse 测试驱动（实验一）
 * make && ./build/led-cmd-parse；全绿打印 ALL PASS。别改本文件。
 *
 * 注意：MQTT 载荷不一定以 '\0' 结尾，长度以 payloadlen 为准。
 * 下面用 memcmp 精确比较长度与字节，所以 "on\0x"（长度 4）必须被拒绝。
 */
#include <stdio.h>
#include <string.h>
#include "led_cmd.h"

static int fails;

static void expect_parse(const char *in, int len, int want)
{
	int got = parse_led_cmd(in, len);
	if (got != want) {
		printf("  FAIL parse(len=%d) got=%d want=%d\n", len, got, want);
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

	/* 合法载荷 */
	static const char on2[2] = { 'o', 'n' };
	static const char off3[3] = { 'o', 'f', 'f' };
	expect_parse(on2, 2, 1);
	expect_parse(off3, 3, 0);

	/* 大小写 / 空格变体 */
	static const char ON2[2] = { 'O', 'N' };
	static const char on_sp[3] = { 'o', 'n', ' ' };
	static const char on_nl[3] = { 'o', 'n', '\n' };
	expect_parse(ON2, 2, -1);
	expect_parse(on_sp, 3, -1);
	expect_parse(on_nl, 3, -1);

	/* 内嵌空字节的伪造载荷：长度对不上就要拒 */
	static const char on_nul_x[4] = { 'o', 'n', '\0', 'x' };
	static const char off_nul[4] = { 'o', 'f', 'f', '\0' };
	expect_parse(on_nul_x, 4, -1);
	expect_parse(off_nul, 4, -1);

	/* 其它 */
	expect_parse("toggle", 6, -1);
	expect_parse("", 0, -1);
	expect_parse(NULL, 0, -1);

	expect_status(1, "on");
	expect_status(0, "off");
	expect_status(42, "on");

	if (fails == 0) {
		printf("  ok   parse 按 payloadlen 精确匹配（含 on\\0x 伪造载荷被拒）\n");
		printf("  ok   status_payload 返回 on/off\n");
		printf("\nALL PASS\n");
		return 0;
	}
	printf("\nSOME FAIL (%d)\n", fails);
	return 1;
}
