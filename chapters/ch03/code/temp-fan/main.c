/*
 * ch03 temp-fan — 本章实验 · 温控风扇（阈值开/关）
 *
 * 修改下方宏以匹配课程接线表。板端需 libgpiod。
 * SIMULATE_SENSOR=1 时不读 DHT，用假温度演示阈值逻辑（无硬件联调时）。
 */
#include <gpiod.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <stdint.h>
#include <string.h>

#define GPIO_CHIP_PATH   "/dev/gpiochip1"
#define DHT_LINE         4 /* IO1_4：DHT22 数据线（经电平转换） */
#define FAN_LINE         3 /* IO1_3：继电器/风扇信号（经电平转换） */

#define T_HIGH           28.0f /* °C：高于此开风扇 */
#define T_LOW            26.0f /* °C：低于此关风扇（滞回） */
#define SAMPLE_MS        2000
#define DHT_RETRY        3

/* 1 = 无传感器时演示阈值；真机读 DHT 改为 0（用户态位带可能偶发失败，属预期） */
#define SIMULATE_SENSOR  1

static struct gpiod_chip *chip;
static struct gpiod_line *fan_line;
static int fan_on;

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
	fan_line = gpiod_chip_get_line(chip, FAN_LINE);
	if (!fan_line) {
		perror("fan get_line");
		return -1;
	}
	if (gpiod_line_request_output(fan_line, "temp-fan", 0) < 0) {
		perror("fan request_output");
		return -1;
	}
	fan_on = 0;
	return 0;
}

static void fan_set(int on)
{
	if (!fan_line)
		return;
	if (gpiod_line_set_value(fan_line, on ? 1 : 0) < 0) {
		perror("fan set_value");
		return;
	}
	fan_on = on;
	printf("[INFO] fan %s\n", on ? "ON" : "OFF");
	fflush(stdout);
}

#if !SIMULATE_SENSOR
/* 简化 DHT22 位带：用户态可能偶发失败，调用方应重试 */
static int dht22_read(float *temp_c, float *hum_pct)
{
	struct gpiod_line *line;
	uint8_t data[5];
	int i, j;
	struct timespec ts_short = { .tv_sec = 0, .tv_nsec = 1000 };      /* 1 us */
	struct timespec ts_30 = { .tv_sec = 0, .tv_nsec = 30000 };
	struct timespec ts_80 = { .tv_sec = 0, .tv_nsec = 80000 };
	struct timespec ts_wait = { .tv_sec = 0, .tv_nsec = 1000000 };  /* 1 ms poll slice */

	memset(data, 0, sizeof(data));
	line = gpiod_chip_get_line(chip, DHT_LINE);
	if (!line)
		return -1;

	if (gpiod_line_request_output(line, "dht22", 1) < 0)
		return -1;

	/* start signal: pull low ~1.1 ms, then high */
	gpiod_line_set_value(line, 0);
	ts_wait.tv_nsec = 1100000;
	nanosleep(&ts_wait, NULL);
	gpiod_line_set_value(line, 1);
	nanosleep(&ts_30, NULL);
	gpiod_line_release(line);

	if (gpiod_line_request_input(line, "dht22") < 0)
		return -1;

	/* wait for sensor response: 80us low + 80us high (loose timeout) */
	{
		int seen_low = 0, seen_high = 0;
		for (i = 0; i < 200; i++) {
			int v = gpiod_line_get_value(line);
			if (v < 0)
				goto fail;
			if (!seen_low && v == 0)
				seen_low = 1;
			else if (seen_low && !seen_high && v == 1)
				seen_high = 1;
			else if (seen_high && v == 0)
				break;
			nanosleep(&ts_short, NULL);
		}
		if (!seen_high)
			goto fail;
	}

	for (j = 0; j < 40; j++) {
		int low_c = 0, high_c = 0;
		while (gpiod_line_get_value(line) == 0) {
			if (++low_c > 100)
				goto fail;
			nanosleep(&ts_short, NULL);
		}
		while (gpiod_line_get_value(line) == 1) {
			if (++high_c > 100)
				goto fail;
			nanosleep(&ts_short, NULL);
		}
		/* high ~70us => 1; ~26us => 0 */
		data[j / 8] <<= 1;
		if (high_c > low_c)
			data[j / 8] |= 1;
	}

	gpiod_line_release(line);

	if (((data[0] + data[1] + data[2] + data[3]) & 0xFF) != data[4])
		return -1;

	*hum_pct = ((data[0] << 8) | data[1]) / 10.0f;
	*temp_c = ((data[2] << 8) | data[3]) / 10.0f;
	if (data[2] & 0x80)
		*temp_c = -(((data[2] & 0x7F) << 8) | data[3]) / 10.0f;
	(void)ts_80;
	return 0;

fail:
	gpiod_line_release(line);
	return -1;
}
#else
static int dht22_read(float *temp_c, float *hum_pct)
{
	static float t = 25.0f;
	static int dir = 1;
	/* 假温度在 24~31 间缓慢变化，便于演示阈值 */
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

int main(void)
{
	float t, h;

	chip = gpiod_chip_open(GPIO_CHIP_PATH);
	if (!chip) {
		perror("gpiod_chip_open");
		return 1;
	}
	if (fan_init() < 0) {
		gpiod_chip_close(chip);
		return 1;
	}

#if SIMULATE_SENSOR
	log_info("SIMULATE_SENSOR=1 — fake temperature ramp");
#else
	log_info("reading DHT22 + threshold fan control (Ctrl+C to stop)");
#endif
	printf("[INFO] T_HIGH=%.1f T_LOW=%.1f sample=%d ms\n",
	       T_HIGH, T_LOW, SAMPLE_MS);

	for (;;) {
		if (read_temp_retry(&t, &h) < 0) {
			log_err("DHT22 read failed — retry next cycle");
		} else {
			printf("[INFO] temp=%.1fC hum=%.1f%% fan=%s\n",
			       t, h, fan_on ? "ON" : "OFF");
			fflush(stdout);
			if (t > T_HIGH && !fan_on)
				fan_set(1);
			else if (t < T_LOW && fan_on)
				fan_set(0);
		}
		usleep((useconds_t)SAMPLE_MS * 1000);
	}

	/* unreachable in normal run */
	gpiod_line_release(fan_line);
	gpiod_chip_close(chip);
	return 0;
}
