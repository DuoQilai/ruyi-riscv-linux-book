/*
 * main.c — 第四章学生版（脚手架）· 工程目录 cmd-thermo/
 *
 * 【学生动手】只改本文件里标了 STUDENT TODO 的区块。
 * 要实现：cmd_status / cmd_set / g_cmds[] / dispatch_command / main_loop
 * 实现前 make 会因未声明的函数失败——这是预期现象。
 *
 * 契约与思考题：chapters/ch04/lab.html
 * 对照答案：main-sol.c（make sol）
 *
 * 板：荔枝派 4A + RevyOS。libgpiod v2。全程 C。
 * 脚：继电器 IO1_5、DHT22 IO1_6（经 TXS）。
 * 命令从 SSH 终端 stdin 读入。
 */
#include <errno.h>
#include <gpiod.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <time.h>
#include <unistd.h>

#define GPIO_CHIP_PATH   "/dev/gpiochip5"
#define DHT_LINE         6   /* IO1_6 */
#define FAN_LINE         5   /* IO1_5 */

#define SAMPLE_MS        2000
#define DHT_RETRY        3
#define LINE_MAX         128

#define H_ON             90.0f
#define H_OFF            72.0f

#ifndef SIMULATE_SENSOR
#define SIMULATE_SENSOR  1
#endif

struct thermo_state {
	float t_high;
	float t_low;
	float last_temp;
	float last_hum;
	int fan_on;
	int has_sample;
};

static struct thermo_state g_st = {
	.t_high = 31.0f,
	.t_low = 29.5f,
	.fan_on = 0,
	.has_sample = 0,
};

static struct gpiod_chip *chip;
static struct gpiod_line_request *fan_req;
static int cmd_fd = -1;
static volatile sig_atomic_t g_running = 1;

static void on_signal(int sig)
{
	(void)sig;
	g_running = 0;
}

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
	gpiod_request_config_set_consumer(rc, "cmd-thermo");

	r = gpiod_chip_request_lines(chip, rc, lc);
	gpiod_request_config_free(rc);
	gpiod_line_config_free(lc);
	gpiod_line_settings_free(s);

	if (r && direction == GPIOD_LINE_DIRECTION_OUTPUT)
		gpiod_line_request_set_value(r, offset, val);
	return r;
}

static void log_info(const char *msg)
{
	printf("[INFO] %s\n", msg);
	fflush(stdout);
}

static void log_err(const char *msg)
{
	fprintf(stderr, "[ERR] %s\n", msg);
	fflush(stderr);
}

static int fan_init(void)
{
	fan_req = line_request(FAN_LINE, GPIOD_LINE_DIRECTION_OUTPUT, 0);
	if (!fan_req) {
		perror("fan request_output");
		return -1;
	}
	g_st.fan_on = 0;
	return 0;
}

static void fan_set(int on)
{
	if (!fan_req)
		return;
	if (gpiod_line_request_set_value(fan_req, FAN_LINE, on ? 1 : 0) < 0) {
		perror("fan set_value");
		return;
	}
	g_st.fan_on = on;
	printf("[INFO] fan %s\n", on ? "ON" : "OFF");
	fflush(stdout);
}

#if !SIMULATE_SENSOR
static int dht22_read(float *temp_c, float *hum_pct)
{
	struct gpiod_line_request *r;
	uint8_t data[5];
	uint8_t bits[40];
	int i, j;
	struct timespec ts_30 = { .tv_sec = 0, .tv_nsec = 30000 };
	struct timespec ts_wait = { .tv_sec = 0, .tv_nsec = 1100000 };

	memset(data, 0, sizeof(data));

	r = line_request(DHT_LINE, GPIOD_LINE_DIRECTION_OUTPUT, 0);
	if (!r)
		return -1;
	nanosleep(&ts_wait, NULL);
	gpiod_line_request_set_value(r, DHT_LINE, 1);
	nanosleep(&ts_30, NULL);
	gpiod_line_request_release(r);

	r = line_request(DHT_LINE, GPIOD_LINE_DIRECTION_INPUT, 0);
	if (!r)
		return -1;
	{
		int seen_low = 0, seen_high = 0;
		for (i = 0; i < 2000; i++) {
			int v = gpiod_line_request_get_value(r, DHT_LINE);
			if (v < 0)
				goto fail;
			if (!seen_low && v == 0)
				seen_low = 1;
			else if (seen_low && !seen_high && v == 1)
				seen_high = 1;
			if (seen_high)
				break;
		}
		if (!seen_high)
			goto fail;
	}

	for (j = 0; j < 40; j++) {
		int low_c, high_c;
		for (;;) {
			while (gpiod_line_request_get_value(r, DHT_LINE) == 1)
				;
			low_c = 0;
			while (gpiod_line_request_get_value(r, DHT_LINE) == 0) {
				if (++low_c > 300)
					goto fail;
			}
			if (low_c >= 5)
				break;
		}
		if (j < 39) {
			high_c = 0;
			while (gpiod_line_request_get_value(r, DHT_LINE) == 1) {
				if (++high_c > 300)
					goto fail;
			}
		} else {
			high_c = 0;
			while (gpiod_line_request_get_value(r, DHT_LINE) == 1 && high_c < 60)
				high_c++;
		}
		bits[j] = (high_c > low_c) ? 1 : 0;
	}
	gpiod_line_request_release(r);

	for (j = 0; j < 5; j++)
		data[j] = 0;
	for (j = 0; j < 40; j++) {
		int b = (j == 0) ? 0 : bits[j - 1];
		data[j / 8] <<= 1;
		data[j / 8] |= b;
	}
	if (((data[0] + data[1] + data[2] + data[3]) & 0xFF) != data[4]) {
		for (j = 0; j < 5; j++)
			data[j] = 0;
		for (j = 0; j < 40; j++) {
			data[j / 8] <<= 1;
			data[j / 8] |= bits[j];
		}
		if (((data[0] + data[1] + data[2] + data[3]) & 0xFF) != data[4])
			return -1;
	}
	*hum_pct = ((data[0] << 8) | data[1]) / 10.0f;
	*temp_c = ((data[2] << 8) | data[3]) / 10.0f;
	if (data[2] & 0x80)
		*temp_c = -(((data[2] & 0x7F) << 8) | data[3]) / 10.0f;
	if (*hum_pct < 0.0f || *hum_pct > 100.0f)
		return -1;
	if (*temp_c < -40.0f || *temp_c > 80.0f)
		return -1;
	return 0;

fail:
	gpiod_line_request_release(r);
	return -1;
}
#else
static int dht22_read(float *temp_c, float *hum_pct)
{
	static float t = 25.0f;
	static int dir = 1;

	t += dir * 0.4f;
	if (t > 31.0f)
		dir = -1;
	if (t < 24.0f)
		dir = 1;
	*temp_c = t;
	*hum_pct = 55.0f;
	return 0;
}
#endif

static int read_temp_retry(float *t, float *h)
{
	int k;
	for (k = 0; k < DHT_RETRY; k++) {
		if (dht22_read(t, h) == 0)
			return 0;
		usleep(100000);
	}
	return -1;
}

/* 脚手架已提供：采样 + 温湿度联合控风扇。你实现命令与 select 主循环。 */
static void sample_and_control(void)
{
	float t, h;

	if (read_temp_retry(&t, &h) < 0) {
		log_err("DHT22 read failed — retry next cycle");
		return;
	}
	g_st.last_temp = t;
	g_st.last_hum = h;
	g_st.has_sample = 1;
	printf("[INFO] temp=%.1fC hum=%.1f%% fan=%s thr=%.1f/%.1f H=%.0f/%.0f\n",
	       t, h, g_st.fan_on ? "ON" : "OFF",
	       g_st.t_high, g_st.t_low, H_ON, H_OFF);
	fflush(stdout);

	/* 开：温度过高 或 湿度过高；关：温度与湿度都回落 */
	if ((t > g_st.t_high || h > H_ON) && !g_st.fan_on)
		fan_set(1);
	else if (t < g_st.t_low && h < H_OFF && g_st.fan_on)
		fan_set(0);
}

/* ========================================================================
 * STUDENT TODO — 在本注释与下方 read_command_line() 之间写下实现
 * ------------------------------------------------------------------------
 * 1) cmd_status(char *args)
 * 2) cmd_set(char *args)
 * 3) struct cmd_entry + g_cmds[]（注册 status / set，表尾 NULL）
 * 4) dispatch_command(char *line)   — 拆词、查表、调用
 * 5) main_loop(void)                — select：超时→sample_and_control；
 *                                    stdin 可读→read_command_line
 *
 * 原型与成功判据只在 lab.html，不要只靠猜。
 * 可调用本文件已有的：sample_and_control / g_st / cmd_fd / g_running …
 * ======================================================================== */

static int read_command_line(void)
{
	char buf[LINE_MAX];
	size_t len;

	if (!fgets(buf, sizeof(buf), stdin))
		return -1;
	len = strlen(buf);
	while (len > 0 && (buf[len - 1] == '\n' || buf[len - 1] == '\r'))
		buf[--len] = '\0';
	dispatch_command(buf);
	return 0;
}

int main(void)
{
	chip = gpiod_chip_open(GPIO_CHIP_PATH);
	if (!chip) {
		perror("gpiod_chip_open");
		return 1;
	}
	if (fan_init() < 0) {
		gpiod_chip_close(chip);
		return 1;
	}

	cmd_fd = STDIN_FILENO;
	log_info("commands on stdin — type: status | set high N | set low N");

#if SIMULATE_SENSOR
	log_info("SIMULATE_SENSOR=1 — fake temperature ramp");
#endif
	printf("[INFO] T_HIGH=%.1f T_LOW=%.1f sample=%d ms\n",
	       g_st.t_high, g_st.t_low, SAMPLE_MS);

	signal(SIGINT, on_signal);
	signal(SIGTERM, on_signal);

	main_loop();

	fan_set(0);
	if (fan_req)
		gpiod_line_request_release(fan_req);
	gpiod_chip_close(chip);
	printf("[INFO] cleaned up, fan off\n");
	return 0;
}
