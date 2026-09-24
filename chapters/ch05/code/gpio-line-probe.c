/*
 * gpio-line-probe — 短暂申请 gpiochip5 指定 line，拉高/拉低并读回。
 * 用法：sudo ./gpio-line-probe <line>   例：sudo ./gpio-line-probe 3
 */
#include <gpiod.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define CHIP_PATH "/dev/gpiochip5"

static struct gpiod_line_request *request_out(struct gpiod_chip *chip,
					      unsigned line, int val)
{
	struct gpiod_line_settings *s = gpiod_line_settings_new();
	struct gpiod_line_config *lc = gpiod_line_config_new();
	struct gpiod_request_config *rc = gpiod_request_config_new();
	struct gpiod_line_request *r;
	unsigned offs[1] = { line };

	if (!s || !lc || !rc)
		return NULL;
	gpiod_line_settings_set_direction(s, GPIOD_LINE_DIRECTION_OUTPUT);
	gpiod_line_settings_set_output_value(
		s, val ? GPIOD_LINE_VALUE_ACTIVE : GPIOD_LINE_VALUE_INACTIVE);
	gpiod_line_config_add_line_settings(lc, offs, 1, s);
	gpiod_request_config_set_consumer(rc, "gpio-line-probe");
	r = gpiod_chip_request_lines(chip, rc, lc);
	gpiod_request_config_free(rc);
	gpiod_line_config_free(lc);
	gpiod_line_settings_free(s);
	return r;
}

int main(int argc, char **argv)
{
	unsigned line = 3;
	struct gpiod_chip *chip;
	struct gpiod_line_request *req;
	enum gpiod_line_value v;

	if (argc >= 2)
		line = (unsigned)atoi(argv[1]);

	chip = gpiod_chip_open(CHIP_PATH);
	if (!chip) {
		perror("gpiod_chip_open");
		return 1;
	}

	req = request_out(chip, line, 0);
	if (!req) {
		fprintf(stderr, "[ERR] request line %u failed\n", line);
		gpiod_chip_close(chip);
		return 1;
	}
	printf("[OK] got line %u as output (initial 0)\n", line);

	if (gpiod_line_request_set_value(req, line, GPIOD_LINE_VALUE_ACTIVE) < 0) {
		perror("set high");
		gpiod_line_request_release(req);
		gpiod_chip_close(chip);
		return 1;
	}
	v = gpiod_line_request_get_value(req, line);
	printf("[OK] set high; readback=%d (expect 1)\n", (int)v);
	usleep(400000);

	if (gpiod_line_request_set_value(req, line, GPIOD_LINE_VALUE_INACTIVE) < 0) {
		perror("set low");
		gpiod_line_request_release(req);
		gpiod_chip_close(chip);
		return 1;
	}
	v = gpiod_line_request_get_value(req, line);
	printf("[OK] set low;  readback=%d (expect 0)\n", (int)v);
	usleep(200000);

	for (int i = 0; i < 2; i++) {
		gpiod_line_request_set_value(req, line, GPIOD_LINE_VALUE_ACTIVE);
		usleep(200000);
		gpiod_line_request_set_value(req, line, GPIOD_LINE_VALUE_INACTIVE);
		usleep(200000);
	}
	printf("[OK] blinked twice; releasing line %u\n", line);

	gpiod_line_request_release(req);
	gpiod_chip_close(chip);
	return 0;
}
