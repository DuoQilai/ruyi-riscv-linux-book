/*
 * fan_off.c — 强制把继电器信号脚拉低（IO1_5 / gpiochip5 line 5）
 *
 * 用途：进程被 kill -9、异常退出或 GPIO 释放后悬浮导致风扇一直转时，
 * 跑一次本程序把脚拉低。根治悬浮仍建议在继电器 IN 到 GND 加 10kΩ 下拉。
 *
 * 编译：make fan-off
 * 运行：sudo ./fan_off
 */
#include <gpiod.h>
#include <stdio.h>
#include <unistd.h>

#define GPIO_CHIP_PATH "/dev/gpiochip5"
#define FAN_LINE       5

int main(void)
{
	struct gpiod_chip *chip;
	struct gpiod_line_settings *s;
	struct gpiod_line_config *lc;
	struct gpiod_request_config *rc;
	struct gpiod_line_request *r;
	unsigned int offs[1] = { FAN_LINE };

	chip = gpiod_chip_open(GPIO_CHIP_PATH);
	if (!chip) {
		perror("gpiod_chip_open");
		return 1;
	}

	s = gpiod_line_settings_new();
	lc = gpiod_line_config_new();
	rc = gpiod_request_config_new();
	if (!s || !lc || !rc) {
		fprintf(stderr, "alloc failed\n");
		return 1;
	}

	gpiod_line_settings_set_direction(s, GPIOD_LINE_DIRECTION_OUTPUT);
	gpiod_line_settings_set_output_value(s, GPIOD_LINE_VALUE_INACTIVE);
	/* 若 SoC 支持，释放后也不易飘高；不支持则忽略 */
	gpiod_line_settings_set_bias(s, GPIOD_LINE_BIAS_PULL_DOWN);
	gpiod_line_config_add_line_settings(lc, offs, 1, s);
	gpiod_request_config_set_consumer(rc, "fan_off");

	r = gpiod_chip_request_lines(chip, rc, lc);
	gpiod_request_config_free(rc);
	gpiod_line_config_free(lc);
	gpiod_line_settings_free(s);

	if (!r) {
		perror("request_lines");
		gpiod_chip_close(chip);
		return 1;
	}

	if (gpiod_line_request_set_value(r, FAN_LINE, 0) < 0)
		perror("set_value");

	/* 短持一会儿，让继电器可靠释放 */
	usleep(200000);

	gpiod_line_request_release(r);
	gpiod_chip_close(chip);
	printf("[INFO] 风扇信号脚已拉低（IO1_5）\n");
	return 0;
}
