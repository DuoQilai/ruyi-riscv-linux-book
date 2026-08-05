/*
 * ch04 serial-thermo — 串口命令温控（本章实验脚手架）
 *
 * 已提供：状态结构、DHT22 读取、继电器/风扇、采样与阈值。
 * 学生 TODO：命令表（status / set）+ select 主循环。
 *
 * 板：荔枝派 4A + RevyOS。libgpiod v2 API。全程 C 语言。
 * 脚位：继电器 IO1_5（gpiochip5 line 5）、DHT22 IO1_6（line 6，经 TXS）。
 *
 * 编译（板端原生）：make ；交叉：make CROSS_COMPILE=riscv64-unknown-linux-gnu-
 */
#include <errno.h>
#include <fcntl.h>
#include <gpiod.h>
#include <signal.h>
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

#define GPIO_CHIP_PATH   "/dev/gpiochip5"
#define DHT_LINE         6   /* IO1_6 */
#define FAN_LINE         5   /* IO1_5 */

#define SAMPLE_MS        2000
#define DHT_RETRY        3
#define LINE_MAX         128

/* 湿度控制：湿度过高（哈气、湿气重）也开风扇；回落后才允许关 */
#define H_ON             90.0f
#define H_OFF            72.0f

/* 1 = 假温度演示；接真 DHT22 用 -DSIMULATE_SENSOR=0 覆盖 */
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
	.t_high = 31.0f,  /* 高于室温：捂热过 31 开风扇 */
	.t_low = 29.5f,   /* 凉回 29.5 以下（且湿度回落）关风扇 */
	.fan_on = 0,
	.has_sample = 0,
};

static struct gpiod_chip *chip;
static struct gpiod_line_request *fan_req;
static int cmd_fd = -1;
static volatile sig_atomic_t g_running = 1;

/* Ctrl+C / kill：只翻旗标，主循环自己收尾（先关风扇再释放 GPIO） */
static void on_signal(int sig)
{
	(void)sig;
	g_running = 0;
}

/* libgpiod v2：申请一根线。direction 为 OUTPUT 时立即置 val。 */
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
	gpiod_request_config_set_consumer(rc, "serial-thermo");

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
/* DHT22 单总线读取（libgpiod v2）。用户态位带可能偶发失败，调用方应重试。 */
static int dht22_read(float *temp_c, float *hum_pct)
{
	struct gpiod_line_request *r;
	uint8_t data[5];
	uint8_t bits[40];
	int i, j;
	struct timespec ts_30 = { .tv_sec = 0, .tv_nsec = 30000 };
	struct timespec ts_wait = { .tv_sec = 0, .tv_nsec = 1100000 };

	memset(data, 0, sizeof(data));

	/* 开始信号：拉低 ~1.1ms，再拉高 ~30us */
	r = line_request(DHT_LINE, GPIOD_LINE_DIRECTION_OUTPUT, 0);
	if (!r)
		return -1;
	nanosleep(&ts_wait, NULL);
	gpiod_line_request_set_value(r, DHT_LINE, 1);
	nanosleep(&ts_30, NULL);
	gpiod_line_request_release(r);

	/* 转输入，等传感器响应：80us 低 + 80us 高（宽松超时） */
	r = line_request(DHT_LINE, GPIOD_LINE_DIRECTION_INPUT, 0);
	if (!r)
		return -1;
	{
		/* 注意：这里不能用 nanosleep(1us)——Linux 上实际睡 30~50us，
		 * 会把 DHT22 的位糊掉。改成忙轮询，get_value 本身 ~2us。 */
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
				break;   /* 响应高电平已见；位循环从下一个真低电平开始 */
		}
		if (!seen_high)
			goto fail;
	}

	/* 读 40 位：低 ~50us，高 26us=0 / 70us=1（忙轮询，不 sleep）。
	 * 三个实测要点：
	 * 1. 不能用 nanosleep(1us)——Linux 上实际睡 30~50us，把位糊掉；
	 * 2. 低电平必须持续 >=5 次采样才算真位——电平转换模块在释放
	 *    瞬间会抖出假低，把假低当位起点会让整个位流错位；
	 * 3. 读完后做「自适应对齐」：原样打包 vs 右移一位打包（首位补 0、
	 *    丢末位），哪个校验通过就用哪个——实测右移版恒为正确数据。 */
	for (j = 0; j < 40; j++) {
		int low_c, high_c;
		for (;;) {
			while (gpiod_line_request_get_value(r, DHT_LINE) == 1)
				;                              /* 等低电平 */
			low_c = 0;
			while (gpiod_line_request_get_value(r, DHT_LINE) == 0) {
				if (++low_c > 300)
					goto fail;
			}
			if (low_c >= 5)
				break;                       /* 真位；短毛刺忽略重等 */
		}
		if (j < 39) {
			high_c = 0;
			while (gpiod_line_request_get_value(r, DHT_LINE) == 1) {
				if (++high_c > 300)
					goto fail;
			}
		} else {
			/* 最后一位：高电平测够 60 次即停（帧结束后线会一直停在
			 * 高电平，不能等它变低，否则误判超时失败） */
			high_c = 0;
			while (gpiod_line_request_get_value(r, DHT_LINE) == 1 && high_c < 60)
				high_c++;
		}
		bits[j] = (high_c > low_c) ? 1 : 0;
	}
	gpiod_line_request_release(r);

	/* 自适应对齐：两种打包，取校验通过者。实测「右移一位」恒为正确数据
	 * （释放瞬间的毛刺让位流错位一位），所以优先右移，原样作后备。 */
	for (j = 0; j < 5; j++)
		data[j] = 0;
	for (j = 0; j < 40; j++) {
		int b = (j == 0) ? 0 : bits[j - 1];
		data[j / 8] <<= 1;
		data[j / 8] |= b;
	}
	if (((data[0] + data[1] + data[2] + data[3]) & 0xFF) != data[4]) {
		/* 原样重打包：bits[0..39] */
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
	/* 合理性过滤：校验和过了但数值离谱（对齐偶发错位）的直接丢弃 */
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

	/* 开：温度过高，或湿度过高（哈气/湿气重），都开风扇 */
	if ((t > g_st.t_high || h > H_ON) && !g_st.fan_on)
		fan_set(1);
	/* 关：温度回落 且 湿度回落，才关风扇 */
	else if (t < g_st.t_low && h < H_OFF && g_st.fan_on)
		fan_set(0);
}

/* ========== 学生 TODO：命令处理 ========== */

/* 打印当前状态：温度、湿度、风扇、阈值；尚无采样时说明清楚 */
static int cmd_status(char *args)
{
	(void)args;
	if (!g_st.has_sample) {
		printf("[INFO] no sample yet — waiting for DHT22\n");
		fflush(stdout);
		return 0;
	}
	printf("[INFO] temp=%.1fC hum=%.1f%% fan=%s thr=%.1f/%.1f H=%.0f/%.0f\n",
	       g_st.last_temp, g_st.last_hum,
	       g_st.fan_on ? "ON" : "OFF",
	       g_st.t_high, g_st.t_low, H_ON, H_OFF);
	fflush(stdout);
	return 0;
}

/* 解析 "high <数>" 或 "low <数>"，写入 g_st；要求 low < high */
static int cmd_set(char *args)
{
	char what[16];
	float v;

	if (sscanf(args, "%15s %f", what, &v) != 2) {
		printf("[ERR] usage: set high <N> | set low <N>\n");
		fflush(stdout);
		return -1;
	}
	if (strcmp(what, "high") == 0) {
		if (v <= g_st.t_low) {
			printf("[ERR] high must be > low (%.1f)\n", g_st.t_low);
			fflush(stdout);
			return -1;
		}
		g_st.t_high = v;
	} else if (strcmp(what, "low") == 0) {
		if (v >= g_st.t_high) {
			printf("[ERR] low must be < high (%.1f)\n", g_st.t_high);
			fflush(stdout);
			return -1;
		}
		g_st.t_low = v;
	} else {
		printf("[ERR] unknown: set %s\n", what);
		fflush(stdout);
		return -1;
	}
	printf("[INFO] threshold -> high=%.1f low=%.1f\n", g_st.t_high, g_st.t_low);
	fflush(stdout);
	return 0;
}

struct cmd_entry {
	const char *name;
	int (*handler)(char *args);
};

static const struct cmd_entry g_cmds[] = {
	{ "status", cmd_status },
	{ "set",    cmd_set    },
	{ NULL,     NULL       },
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
#endif
	len = strlen(buf);
	while (len > 0 && (buf[len - 1] == '\n' || buf[len - 1] == '\r'))
		buf[--len] = '\0';
	dispatch_command(buf);
	return 0;
}

/* 用 select 同时等命令可读与采样超时：
 * 超时 → 采样控温；命令可读 → 处理一行。
 * deadline 用墙上时钟算「距下次采样还剩多少」，处理命令后不重置，
 * 采样节奏就不会被打字拖慢。 */
static void main_loop(void)
{
	double deadline = 0.0;
	struct timespec ts;
	int cmd_open = 1;   /* stdin 读到 EOF 后不再等命令，专心采样 */

	while (g_running) {
		fd_set rfds;
		struct timeval tv;
		int n;

		clock_gettime(CLOCK_MONOTONIC, &ts);
		double now_s = ts.tv_sec + ts.tv_nsec / 1e9;
		if (deadline <= now_s)
			deadline = now_s + SAMPLE_MS / 1000.0;
		double remain = deadline - now_s;
		tv.tv_sec = (long)remain;
		tv.tv_usec = (long)((remain - tv.tv_sec) * 1e6);

		FD_ZERO(&rfds);
		if (cmd_open)
			FD_SET(cmd_fd, &rfds);
		n = select(cmd_fd + 1, &rfds, NULL, NULL, &tv);
		if (n < 0) {
			if (errno == EINTR)
				break;               /* Ctrl+C 打断了 select */
			perror("select");
			break;
		}
		if (n == 0) {
			sample_and_control();           /* 超时：该采样了 */
		} else if (cmd_open && FD_ISSET(cmd_fd, &rfds)) {
			if (read_command_line() < 0)
				cmd_open = 0;               /* EOF：输入结束 */
		}
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

	signal(SIGINT, on_signal);
	signal(SIGTERM, on_signal);

	main_loop();

	/* 优雅收尾：先把继电器拉低（模块输入悬浮会自己飘高），再释放 */
	fan_set(0);
	if (fan_req)
		gpiod_line_request_release(fan_req);
	gpiod_chip_close(chip);
#if !USE_STDIO
	close(cmd_fd);
#endif
	printf("[INFO] cleaned up, fan off\n");
	return 0;
}
