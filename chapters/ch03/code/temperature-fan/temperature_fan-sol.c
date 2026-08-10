/*
 * temperature_fan-sol.c — 第三章参考实现（solution）
 *
 * 默认 make 编的是 temperature_fan.c（学生版）。
 * 对照本文件：make sol && sudo ./temperature_fan-sol
 *
 * 板：荔枝派 4A + RevyOS。libgpiod v2。全程 C。
 * 脚：IO1_5 → gpiochip5 line 5 → 继电器信号端（直连）。
 */
#include <gpiod.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>

#define GPIO_CHIP_PATH   "/dev/gpiochip5"
#define FAN_LINE         5   /* IO1_5 */

#define T_HIGH           28.0f
#define T_LOW            26.0f
#define STEP_MS          250
#define STEP_DELTA       0.5f

static struct gpiod_chip *chip;
static struct gpiod_line_request *fan_req;
static volatile sig_atomic_t g_running = 1;

static void on_signal(int sig)
{
	(void)sig;
	g_running = 0;
}

/* 申请一根脚：direction 为 OUTPUT 时立刻置 val（0/1） */
struct gpiod_line_request *line_request(unsigned int offset,
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

/* 写继电器信号端：on=1 吸合，on=0 释放；并打日志 */
void fan_set(int on)
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

/*
 * 滞回决策：只改 *fan_on，不碰 GPIO。
 *   temp > T_HIGH → 开
 *   temp < T_LOW  → 关
 *   中间区间      → 保持 *fan_on
 */
void fan_decide(float temp_c, int *fan_on)
{
	if (!fan_on)
		return;
	if (temp_c > T_HIGH)
		*fan_on = 1;
	else if (temp_c < T_LOW)
		*fan_on = 0;
}

int main(void)
{
	float t = 24.0f;
	int dir = 1;
	int fan = 0;
	int prev;

	signal(SIGINT, on_signal);
	signal(SIGTERM, on_signal);

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

	while (g_running) {
		t += dir * STEP_DELTA;
		if (t > 31.0f)
			dir = -1;
		if (t < 24.0f)
			dir = 1;

		prev = fan;
		fan_decide(t, &fan);
		if (fan != prev)
			fan_set(fan);

		printf("[INFO] temp=%.1fC fan=%s\n", t, fan ? "ON" : "OFF");
		fflush(stdout);
		usleep((useconds_t)STEP_MS * 1000);
	}

	/* 显式拉低再释放：本模块输入悬浮可能飘高，风扇会一直转 */
	fan_set(0);
	gpiod_line_request_release(fan_req);
	gpiod_chip_close(chip);
	printf("[INFO] cleaned up, fan off\n");
	return 0;
}
