/*
 * ch04 serial-thermo — 串口命令温控（本章实验脚手架）
 *
 * 已提供：状态结构、DHT/风扇、采样与阈值。
 * 学生 TODO：命令表（status / set）+ select 主循环。
 *
 * 板卡：荔枝派 4A。工具链：RuyiSDK（riscv64-unknown-linux-gnu-gcc）。
 */
#include <errno.h>
#include <fcntl.h>
#include <gpiod.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <time.h>
#include <unistd.h>

/* 1 = SSH 终端 stdin/stdout；0 = 打开真实串口设备 */
#define USE_STDIO        1
#define UART_DEV         "/dev/ttyS1"

#define GPIO_CHIP_PATH   "/dev/gpiochip0"
#define DHT_LINE         18 /* TODO: 按接线表改 */
#define FAN_LINE         17 /* TODO: 按接线表改 */

#define SAMPLE_MS        2000
#define DHT_RETRY        3
#define LINE_MAX         128

/* 1 = 假温度演示；接真 DHT22 改为 0 */
#define SIMULATE_SENSOR  1

struct thermo_state {
	float t_high;
	float t_low;
	float last_temp;
	float last_hum;
	int fan_on;
	int has_sample;
};

static struct thermo_state g_st = {
	.t_high = 28.0f,
	.t_low = 26.0f,
	.fan_on = 0,
	.has_sample = 0,
};

static struct gpiod_chip *chip;
static struct gpiod_line *fan_line;
static int cmd_fd = -1;

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
	if (gpiod_line_request_output(fan_line, "serial-thermo", 0) < 0) {
		perror("fan request_output");
		return -1;
	}
	g_st.fan_on = 0;
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
	g_st.fan_on = on;
	printf("[INFO] fan %s\n", on ? "ON" : "OFF");
	fflush(stdout);
}

#if !SIMULATE_SENSOR
static int dht22_read(float *temp_c, float *hum_pct)
{
	struct gpiod_line *line;
	uint8_t data[5];
	int i, j;
	struct timespec ts_short = { .tv_sec = 0, .tv_nsec = 1000 };
	struct timespec ts_30 = { .tv_sec = 0, .tv_nsec = 30000 };
	struct timespec ts_wait = { .tv_sec = 0, .tv_nsec = 1100000 };

	memset(data, 0, sizeof(data));
	line = gpiod_chip_get_line(chip, DHT_LINE);
	if (!line)
		return -1;
	if (gpiod_line_request_output(line, "dht22", 1) < 0)
		return -1;

	gpiod_line_set_value(line, 0);
	nanosleep(&ts_wait, NULL);
	gpiod_line_set_value(line, 1);
	nanosleep(&ts_30, NULL);
	gpiod_line_release(line);

	if (gpiod_line_request_input(line, "dht22") < 0)
		return -1;

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
	printf("[INFO] temp=%.1fC hum=%.1f%% fan=%s thr=%.1f/%.1f\n",
	       t, h, g_st.fan_on ? "ON" : "OFF", g_st.t_high, g_st.t_low);
	fflush(stdout);

	if (t > g_st.t_high && !g_st.fan_on)
		fan_set(1);
	else if (t < g_st.t_low && g_st.fan_on)
		fan_set(0);
}

/* ========== 学生 TODO：命令处理 ========== */

/*
 * TODO: 打印 g_st 中的温度、湿度、风扇、阈值。
 * 无有效采样时也要说明「尚无采样」。
 */
static int cmd_status(char *args)
{
	(void)args;
	/* TODO: 实现 status */
	printf("[TODO] cmd_status not implemented\n");
	fflush(stdout);
	return -1;
}

/*
 * TODO: 解析 "high <数>" 或 "low <数>"，写入 g_st。
 * 要求 low < high；失败打印 [ERR] 并返回非 0。
 */
static int cmd_set(char *args)
{
	(void)args;
	/* TODO: 实现 set high / set low */
	printf("[TODO] cmd_set not implemented\n");
	fflush(stdout);
	return -1;
}

struct cmd_entry {
	const char *name;
	int (*handler)(char *args);
};

/*
 * TODO: 把 status / set 挂进表（末尾用 {NULL,NULL} 结束）。
 */
static const struct cmd_entry g_cmds[] = {
	/* TODO: { "status", cmd_status }, */
	/* TODO: { "set",    cmd_set }, */
	{ NULL, NULL },
};

/* 按名字查找并调用；未知命令返回 -1 */
static int dispatch_command(char *line)
{
	char *cmd, *args;
	const struct cmd_entry *e;

	while (*line == ' ' || *line == '\t')
		line++;
	if (*line == '\0' || *line == '\n')
		return 0;

	cmd = line;
	args = cmd;
	while (*args && *args != ' ' && *args != '\t' && *args != '\n')
		args++;
	if (*args) {
		*args = '\0';
		args++;
		while (*args == ' ' || *args == '\t')
			args++;
	}

	for (e = g_cmds; e->name; e++) {
		if (strcmp(e->name, cmd) == 0)
			return e->handler(args);
	}
	printf("[ERR] unknown command: %s\n", cmd);
	fflush(stdout);
	return -1;
}

static int read_command_line(void)
{
	char buf[LINE_MAX];
	ssize_t n;
	size_t len;

#if USE_STDIO
	if (!fgets(buf, sizeof(buf), stdin))
		return -1; /* EOF */
#else
	n = read(cmd_fd, buf, sizeof(buf) - 1);
	if (n <= 0)
		return -1;
	buf[n] = '\0';
	/* 简化：假定一次 read 含一行；正式实现可做行缓冲 */
#endif
	len = strlen(buf);
	while (len > 0 && (buf[len - 1] == '\n' || buf[len - 1] == '\r'))
		buf[--len] = '\0';
	dispatch_command(buf);
	return 0;
}

/*
 * TODO: 用 select 同时等 cmd_fd 可读 与 采样超时。
 * 超时 → sample_and_control()；可读 → read_command_line()。
 * 提示：处理命令后应扣减剩余等待时间，避免采样越来越稀。
 */
static void main_loop(void)
{
	/* 占位：阻塞式「先采样再睡」，两边不能同时工作。
	 * 请改成 select 版本（见讲义 4.3）。 */
	log_info("TODO: replace this loop with select()");
	for (;;) {
		sample_and_control();
		/* TODO: 不要只用 sleep；改为 select + 剩余超时 */
		usleep((useconds_t)SAMPLE_MS * 1000);
		/* TODO: 在 select 可读分支里调用 read_command_line() */
		(void)cmd_fd;
		(void)read_command_line; /* 防未使用告警；实现后删除此行 */
	}
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

#if USE_STDIO
	cmd_fd = STDIN_FILENO;
	log_info("USE_STDIO=1 — type: status | set high N | set low N");
#else
	cmd_fd = open(UART_DEV, O_RDWR | O_NOCTTY);
	if (cmd_fd < 0) {
		perror("open UART");
		gpiod_chip_close(chip);
		return 1;
	}
	printf("[INFO] commands on %s\n", UART_DEV);
#endif

#if SIMULATE_SENSOR
	log_info("SIMULATE_SENSOR=1 — fake temperature ramp");
#endif
	printf("[INFO] T_HIGH=%.1f T_LOW=%.1f sample=%d ms\n",
	       g_st.t_high, g_st.t_low, SAMPLE_MS);

	main_loop();

	if (fan_line)
		gpiod_line_release(fan_line);
	gpiod_chip_close(chip);
#if !USE_STDIO
	close(cmd_fd);
#endif
	return 0;
}
