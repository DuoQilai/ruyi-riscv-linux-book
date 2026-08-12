/*
 * fan_off.c — 强制关风扇（继电器信号脚 IO1_5 / gpiochip5 line 5）
 *
 * 为何需要：kill -9 / 崩溃后 GPIO 请求被内核收回，脚变高阻悬浮，
 * 继电器 IN 易被当成高电平，风扇继续转。正常 Ctrl+C 路径已在主程序里先拉低。
 *
 * 实践优先级：
 *   1) 硬件：IN → GND 加 10kΩ 下拉（唯一真正干净的 fail-safe）
 *   2) 软件：本工具主动驱动为低并保持占用，直到 Ctrl+C
 *      （退出后若不加下拉，脚仍可能再次悬浮）
 *
 * 用法：
 *   make fan-off && sudo ./fan_off          # 保持关断，Ctrl+C 结束
 *   make fan-off && sudo ./fan_off --once   # 拉低约 200ms 后退出
 */
#include <gpiod.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define GPIO_CHIP_PATH "/dev/gpiochip5"
#define FAN_LINE       5

static volatile sig_atomic_t g_stop = 0;

static void on_signal(int sig)
{
	(void)sig;
	g_stop = 1;
}

int main(int argc, char **argv)
{
	struct gpiod_chip *chip;
	struct gpiod_line_settings *s;
	struct gpiod_line_config *lc;
	struct gpiod_request_config *rc;
	struct gpiod_line_request *r;
	unsigned int offs[1] = { FAN_LINE };
	int once = 0;
	int i;

	for (i = 1; i < argc; i++) {
		if (strcmp(argv[i], "--once") == 0)
			once = 1;
		else if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
			printf("Usage: %s [--once]\n", argv[0]);
			printf("  (default) hold IO1_5 low until Ctrl+C\n");
			printf("  --once    drive low ~200ms then exit\n");
			return 0;
		} else {
			fprintf(stderr, "unknown option: %s (try --help)\n", argv[i]);
			return 1;
		}
	}

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
	/* 尽力请求下拉；SoC 不支持则忽略 */
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

	if (once) {
		usleep(200000);
		printf("[INFO] 已拉低 IO1_5（--once）；无仍转请加 10kΩ 下拉或去掉 --once 保持占用\n");
	} else {
		signal(SIGINT, on_signal);
		signal(SIGTERM, on_signal);
		printf("[INFO] 已关断并占用 IO1_5；Ctrl+C 结束（根治：继电器 IN→GND 10kΩ 下拉）\n");
		fflush(stdout);
		while (!g_stop)
			sleep(1);
		/* 退出前再确认一次低电平 */
		gpiod_line_request_set_value(r, FAN_LINE, 0);
		usleep(50000);
		printf("[INFO] 已释放 GPIO；若无无下拉，风扇可能再次自启\n");
	}

	gpiod_line_request_release(r);
	gpiod_chip_close(chip);
	return 0;
}
