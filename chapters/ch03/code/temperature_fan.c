/*
 * temperature_fan.c — 第三章学生版（脚手架）· chapters/ch03/code/
 *
 * 【学生动手】只改本文件里标了 STUDENT TODO 的区块。
 * 要实现：line_request / fan_set / fan_decide（写在 main 上方）
 * 实现前 make 会因「未声明的函数」失败——这是预期现象。
 *
 * 契约与思考题：chapters/ch03/lab.html
 * 对照答案：temperature_fan-sol.c（make sol）
 *
 * 板：荔枝派 4A + RevyOS。libgpiod v2。模拟温度控制继电器/风扇。
 * 脚：IO1_5 → gpiochip5 line 5 → 继电器信号端（直连）。
 */
#include <gpiod.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>

#define GPIO_CHIP_PATH   "/dev/gpiochip5"
#define FAN_LINE         5   /* IO1_5 */

#define T_HIGH           29.0f
#define T_LOW            25.0f
#define STEP_MS          250
#define STEP_DELTA       0.5f
#define T_SWEEP_MIN      24.0f
#define T_SWEEP_MAX      30.0f

static struct gpiod_chip *chip;
static struct gpiod_line_request *fan_req;
static volatile sig_atomic_t g_running = 1;

static void on_signal(int sig)
{
	(void)sig;
	g_running = 0;
}

/* ========================================================================
 * STUDENT TODO — 在本注释与下方 main() 之间写下三个函数
 * ------------------------------------------------------------------------
 * 1) line_request(offset, direction, val)
 * 2) fan_set(on)
 * 3) fan_decide(temp_c, fan_on)
 *
 * 完整原型、参数契约与思考题只在 lab.html。
 * ======================================================================== */

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

	printf("[INFO] 模拟温度控制: T_HIGH=%.1f T_LOW=%.1f\n",
	       T_HIGH, T_LOW);
	printf("[INFO] relay/fan on IO1_5 (gpiochip5 line 5); Ctrl+C to stop\n");

	while (g_running) {
		/* 到边界就反向：模拟温度保持在 [T_SWEEP_MIN, T_SWEEP_MAX] 内 */
		if (dir > 0 && t + STEP_DELTA > T_SWEEP_MAX) {
			t = T_SWEEP_MAX;
			dir = -1;
		} else if (dir < 0 && t - STEP_DELTA < T_SWEEP_MIN) {
			t = T_SWEEP_MIN;
			dir = 1;
		} else {
			t += dir * STEP_DELTA;
		}

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
