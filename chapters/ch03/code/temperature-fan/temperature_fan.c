/*
 * ch03 temperature_fan — 温控风扇（第三章 GPIO 输出，无传感器）
 *
 * 只讲 GPIO 输出：假温度 + 阈值逻辑 → 控制继电器 → 风扇。
 * 不读 DHT22（真传感器在第四章，DHT22 已挪到那边）。
 *
 * 板：荔枝派 4A + RevyOS。libgpiod v2 API。全程 C 语言（不用 C++）。
 * 脚位：IO1_5 → gpiochip5 line 5 → 继电器信号端（直连）。
 *       （本板 IO1_3 实测拉不高，已在讲义「前沿板卡」一节说明。）
 *
 * 编译（板端原生）：make
 * 交叉编译（主机）：make CROSS_COMPILE=riscv64-unknown-linux-gnu-
 */
#include <gpiod.h>
#include <stdio.h>
#include <unistd.h>

#define GPIO_CHIP_PATH   "/dev/gpiochip5"
#define FAN_LINE         5   /* IO1_5 */

#define T_HIGH           28.0f
#define T_LOW            26.0f
#define STEP_MS          250
#define STEP_DELTA       0.4f

static struct gpiod_chip *chip;
static struct gpiod_line_request *fan_req;

/* 用 libgpiod v2 申请一根输出线：direction=OUTPUT 时立即置 val */
static struct gpiod_line_request *line_request(unsigned int offset,
                                               int direction, int val)
{
	struct gpiod_line_settings *s = gpiod_line_settings_new();
	struct gpiod_line_config *lc = gpiod_line_config_new();
	struct gpiod_request_config *rc = gpiod_request_config_new();
	struct gpiod_line_request *r;
	unsigned int offs[1] = { offset };

	if (!s || !lc || !rc)
		return NULL;
	gpiod_line_settings_set_direction(s, direction);
	gpiod_line_config_add_line_settings(lc, offs, 1, s);
	gpiod_request_config_set_consumer(rc, "temperature-fan");

	r = gpiod_chip_request_lines(chip, rc, lc);
	gpiod_request_config_free(rc);
	gpiod_line_config_free(lc);
	gpiod_line_settings_free(s);

	if (r && direction == GPIOD_LINE_DIRECTION_OUTPUT)
		gpiod_line_request_set_value(r, offset, val);
	return r;
}

static void fan_set(int on)
{
	if (!fan_req)
		return;
	if (gpiod_line_request_set_value(fan_req, FAN_LINE, on ? 1 : 0) < 0) {
		perror("fan set_value");
		return;
	}
	printf("[INFO] fan %s\n", on ? "ON" : "OFF");
	fflush(stdout);
}

int main(void)
{
	float t = 24.0f;
	int dir = 1;
	int fan = 0;

	chip = gpiod_chip_open(GPIO_CHIP_PATH);
	if (!chip) {
		perror("gpiod_chip_open");
		return 1;
	}
	fan_req = line_request(FAN_LINE, GPIOD_LINE_DIRECTION_OUTPUT, 0);
	if (!fan_req) {
		perror("line_request output");
		gpiod_chip_close(chip);
		return 1;
	}

	printf("[INFO] fake temperature: T_HIGH=%.1f T_LOW=%.1f\n",
	       T_HIGH, T_LOW);
	printf("[INFO] relay/fan on IO1_5 (gpiochip5 line 5); Ctrl+C to stop\n");

	for (;;) {
		t += dir * STEP_DELTA;
		if (t > 31.0f)
			dir = -1;
		if (t < 24.0f)
			dir = 1;

		if (t > T_HIGH && !fan) {
			fan_set(1);
			fan = 1;
		} else if (t < T_LOW && fan) {
			fan_set(0);
			fan = 0;
		}

		printf("[INFO] temp=%.1fC fan=%s\n", t, fan ? "ON" : "OFF");
		fflush(stdout);
		usleep((useconds_t)STEP_MS * 1000);
	}

	return 0;
}
