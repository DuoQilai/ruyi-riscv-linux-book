/*
 * ch06 tri-thread — 三线程协同（产品骨架）
 *
 * 采集 ∥ 控制 ∥ 通信(MQTT)
 * USE_LOCK=0 时不加锁，便于板上必现错乱；验收改为 1。
 *
 * 板：荔枝派 4A + RevyOS。libgpiod v2 API。全程 C 语言。
 * 脚位：继电器 IO1_5（gpiochip5 line 5）、DHT22 IO1_6（line 6，经 TXS）。
 *
 * 编译（板端原生）：make ；交叉：make CROSS_COMPILE=riscv64-ruyisdk-linux-gnu-
 */
#include <errno.h>
#include <gpiod.h>
#include <mosquitto.h>
#include <pthread.h>
#include <signal.h>
#include <stdatomic.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#ifndef USE_LOCK
#define USE_LOCK         0 /* 实验：先 0 看 [RACE]，再改 1 验收 */
#endif
#define SIMULATE_SENSOR  1

#define DEFAULT_BROKER_HOST "192.168.1.10"
#define BROKER_PORT      1883
#define TOPIC_CMD        "course/thermo/cmd"
#define TOPIC_STATUS     "course/thermo/status"
#define DEFAULT_CLIENT_ID "licheepi4a-tri-thread"

static char client_id_buf[128];

/* MQTT_CLIENT_ID 优先；否则 tri-thread-<hostname>，避免多板同 ID 互踢 */
static const char *resolve_client_id(void)
{
	const char *env = getenv("MQTT_CLIENT_ID");
	char host[64];

	if (env && env[0])
		return env;
	if (gethostname(host, sizeof(host)) == 0 && host[0]) {
		snprintf(client_id_buf, sizeof(client_id_buf),
			 "tri-thread-%s", host);
		return client_id_buf;
	}
	return DEFAULT_CLIENT_ID;
}

#define GPIO_CHIP_PATH   "/dev/gpiochip5"
#define DHT_LINE         6  /* IO1_6 */
#define FAN_LINE         5  /* IO1_5 */
#define SAMPLE_MS        500
#define CONTROL_MS       50
/* USE_LOCK=0 时两字段之间故意拉开窗口，让控制线程必能抓到不一致快照 */
#define RACE_WINDOW_US   80000
/* MQTT 断线重连的连续失败上限：到顶就收线程，避免无限空转 */
#define MQTT_MAX_RETRY   5

struct shared_state {
	pthread_mutex_t lock;
	/* 同一次采样应写入相同的一对值；无锁时中间留空窗 → 控制线程读到不一致即 [RACE] */
	int temp_a;
	int temp_b;
	float t_high;
	float t_low;
	float hum;
	int fan_on;
	/* 0 滞回自动控制；1 强制开；2 强制关。综合项目 set_fan 用后两种。 */
	int fan_mode;
	int has_sample;
	int race_hits;
	/* 退出标志：信号与各线程共享；用原子避免 TSan/数据竞争 */
	atomic_int running;
};

static struct shared_state g_st;
static struct gpiod_chip *chip;
static struct gpiod_line_request *fan_req;
static struct mosquitto *mosq;

/* libgpiod v2：申请一根线。direction 为 OUTPUT 时立即置 val。 */
static struct gpiod_line_request *line_request(unsigned int offset,
                                               int direction, int val)
{
	struct gpiod_line_settings *s = gpiod_line_settings_new();
	struct gpiod_line_config *lc = gpiod_line_config_new();
	struct gpiod_request_config *rc = gpiod_request_config_new();
	struct gpiod_line_request *r;
	unsigned int offs[1] = { offset };

	if (!s || !lc || !rc) {
		if (rc)
			gpiod_request_config_free(rc);
		if (lc)
			gpiod_line_config_free(lc);
		if (s)
			gpiod_line_settings_free(s);
		return NULL;
	}
	gpiod_line_settings_set_direction(s, direction);
	gpiod_line_config_add_line_settings(lc, offs, 1, s);
	gpiod_request_config_set_consumer(rc, "tri-thread");

	r = gpiod_chip_request_lines(chip, rc, lc);
	gpiod_request_config_free(rc);
	gpiod_line_config_free(lc);
	gpiod_line_settings_free(s);

	if (r && direction == GPIOD_LINE_DIRECTION_OUTPUT) {
		/* 初次置值失败就把请求还回去：否则调用方以为线已经拉到 val */
		if (gpiod_line_request_set_value(r, offset, val) < 0) {
			fprintf(stderr, "[ERR] line_request: initial set_value failed\n");
			gpiod_line_request_release(r);
			return NULL;
		}
	}
	return r;
}

static void state_lock(void)
{
#if USE_LOCK
	pthread_mutex_lock(&g_st.lock);
#endif
}

static void state_unlock(void)
{
#if USE_LOCK
	pthread_mutex_unlock(&g_st.lock);
#endif
}

static void on_sigint(int sig)
{
	(void)sig;
	/* 信号处理器内仅做无锁原子写；各线程用 load 观察 */
	atomic_store_explicit(&g_st.running, 0, memory_order_relaxed);
}

static int running_load(void)
{
	return atomic_load_explicit(&g_st.running, memory_order_relaxed);
}

static char g_broker_host[64];

/* 重连 Broker；成功返回 0 */
static int mqtt_reconnect(void)
{
	int rc;

	if (!mosq)
		return -1;
	rc = mosquitto_reconnect(mosq);
	if (rc != MOSQ_ERR_SUCCESS)
		fprintf(stderr, "[ERR] reconnect: %s\n", mosquitto_strerror(rc));
	else
		printf("[MQTT] reconnected %s\n", g_broker_host);
	return rc == MOSQ_ERR_SUCCESS ? 0 : -1;
}

static int fan_init(void)
{
	fan_req = line_request(FAN_LINE, GPIOD_LINE_DIRECTION_OUTPUT, 0);
	if (!fan_req)
		return -1;
	return 0;
}

/* 返回 0 成功；写入失败不改 fan_on，避免误报已关风扇 */
static int fan_set_unlocked(int on)
{
	int rc;

	if (!fan_req) {
		fprintf(stderr, "[ERR] fan_set: no GPIO request\n");
		return -1;
	}
	rc = gpiod_line_request_set_value(fan_req, FAN_LINE,
					  on ? GPIOD_LINE_VALUE_ACTIVE
					     : GPIOD_LINE_VALUE_INACTIVE);
	if (rc < 0) {
		fprintf(stderr, "[ERR] fan_set: GPIO write failed (on=%d)\n", on);
		return -1;
	}
	g_st.fan_on = on;
	printf("[INFO] fan %s\n", on ? "ON" : "OFF");
	fflush(stdout);
	return 0;
}

#if SIMULATE_SENSOR
static int dht22_read(float *t, float *h)
{
	static float temp = 25.0f;
	static int dir = 1;
	temp += dir * 0.5f;
	if (temp > 31.0f)
		dir = -1;
	if (temp < 24.0f)
		dir = 1;
	*t = temp;
	*h = 55.0f;
	return 0;
}
#else
static int dht22_read(float *t, float *h)
{
	/* TODO: 复用第四章的 DHT22 位带；此处留桩 */
	(void)t;
	(void)h;
	(void)DHT_LINE;
	return -1;
}
#endif

/* ---------- 采集线程 ---------- */
static void *thread_sense(void *arg)
{
	(void)arg;
	while (running_load()) {
		float t = 0, h = 0;
		if (dht22_read(&t, &h) == 0) {
			int v = (int)(t * 10.0f); /* 0.1℃ 为单位，便于整数比对 */

			state_lock();
			g_st.temp_a = v;
#if !USE_LOCK
			/*
			 * 故意不在同一临界区内完成成对写入：中间 sleep，
			 * 让控制线程几乎必读到 temp_a != temp_b。
			 */
			state_unlock();
			usleep(RACE_WINDOW_US);
			state_lock();
#endif
			g_st.temp_b = v;
			g_st.hum = h;
			g_st.has_sample = 1;
			state_unlock();
			printf("[SENSE] temp=%.1f\n", t);
			fflush(stdout);
		} else {
			fprintf(stderr, "[ERR] sense read failed\n");
		}
		usleep((useconds_t)SAMPLE_MS * 1000);
	}
	return NULL;
}

/* ---------- 控制线程 ---------- */
static void *thread_control(void *arg)
{
	(void)arg;
	while (running_load()) {
		int a, b, has, raced = 0;

		state_lock();
		a = g_st.temp_a;
		b = g_st.temp_b;
		has = g_st.has_sample;

		if (has && a != b) {
			g_st.race_hits++; /* 仅统计；打印在锁外当证据 */
			raced = 1;
		} else if (has && a == b) {
			/*
			 * 判定与执行必须同处一把锁：若先读快照、解锁、再按旧值动作，
			 * MQTT 刚下发的 fan on/off（改 fan_mode/fan_on）会被这一步
			 * 基于旧状态的自动控制覆盖掉。
			 */
			int fan = g_st.fan_on;
			int mode = g_st.fan_mode;
			float high = g_st.t_high;
			float low = g_st.t_low;
			float t = (float)a / 10.0f;

			if (mode == 1 && !fan)
				fan_set_unlocked(1);
			else if (mode == 2 && fan)
				fan_set_unlocked(0);
			else if (mode == 0 && t > high && !fan)
				fan_set_unlocked(1);
			else if (mode == 0 && t < low && fan)
				fan_set_unlocked(0);
		}
		state_unlock();

		if (raced) {
			printf("[RACE] inconsistent snapshot temp_a=%d temp_b=%d\n",
			       a, b);
			fflush(stdout);
		}
		usleep((useconds_t)CONTROL_MS * 1000);
	}
	return NULL;
}

/* ---------- 通信线程（MQTT） ---------- */

/*
 * 订阅失败只置标志，不在这里重连：on_connect 是在 mosquitto_loop() 里被回调的，
 * 重入 libmosquitto 的重连路径不安全。通信线程看到标志后走断线重连那条路。
 */
static int g_sub_failed;

static void on_connect(struct mosquitto *m, void *obj, int rc)
{
	int src;

	(void)obj;
	if (rc != 0) {
		fprintf(stderr, "[ERR] CONNACK rc=%d (broker refused)\n", rc);
		fflush(stderr);
		return;
	}
	src = mosquitto_subscribe(m, NULL, TOPIC_CMD, 0);
	if (src != MOSQ_ERR_SUCCESS) {
		/* 不能只打一行「subscribed」就完事：那样板子看着连上了，命令却永远收不到 */
		fprintf(stderr, "[ERR] subscribe %s: %s\n", TOPIC_CMD,
			mosquitto_strerror(src));
		fflush(stderr);
		g_sub_failed = 1;
		return;
	}
	printf("[MQTT] subscribed %s\n", TOPIC_CMD);
	fflush(stdout);
}

/*
 * 按 payloadlen 精确比较：载荷必须与命令等长且逐字节相同。
 * 不能先把载荷当 C 字符串再 strcmp —— "fan on\0junk" 会被截断成合法的 "fan on"，
 * 于是带尾巴的伪造载荷能绕过命令格式（第五章 mqtt-led 修过同一个洞）。
 */
static int payload_is(const struct mosquitto_message *msg, const char *cmd)
{
	size_t n = strlen(cmd);

	return msg->payloadlen == (int)n && memcmp(msg->payload, cmd, n) == 0;
}

/* 取 "set high "/"set low " 之后的数值：做有界副本，含内嵌 NUL 或超长一律判非法 */
static int payload_number(const struct mosquitto_message *msg, size_t off,
			  double *out)
{
	size_t n;
	char tail[32];
	char *end;

	if (off >= (size_t)msg->payloadlen)
		return -1;
	n = (size_t)msg->payloadlen - off;
	if (n + 1 > sizeof(tail))
		return -1;
	if (memchr((const char *)msg->payload + off, '\0', n))
		return -1;
	memcpy(tail, (const char *)msg->payload + off, n);
	tail[n] = '\0';
	*out = strtod(tail, &end);
	return (*end == '\0' && end != tail) ? 0 : -1;
}

static void on_message(struct mosquitto *m, void *obj,
		       const struct mosquitto_message *msg)
{
	(void)m;
	(void)obj;
	char buf[64];
	double v;

	if (!msg || !msg->payload)
		return;
	snprintf(buf, sizeof(buf), "%.*s",
		 msg->payloadlen > 63 ? 63 : msg->payloadlen,
		 (const char *)msg->payload);
	printf("[MQTT] cmd payload=%s (len=%d)\n", buf, msg->payloadlen);
	fflush(stdout);

	/*
	 * 命令（综合项目工具只转发这些字符串，不直接碰 GPIO）：
	 *   set high 30 / set low 25
	 *   fan on / fan off / fan auto
	 *   status → 发布到 TOPIC_STATUS
	 */
	if (payload_is(msg, "status")) {
		int ta, tb, fan, mode, has;
		float high, low, hum;
		char line[160];
		const char *fan_s;
		const char *mode_s;

		state_lock();
		ta = g_st.temp_a;
		tb = g_st.temp_b;
		hum = g_st.hum;
		high = g_st.t_high;
		low = g_st.t_low;
		fan = g_st.fan_on;
		mode = g_st.fan_mode;
		has = g_st.has_sample;
		state_unlock();
		fan_s = fan ? "on" : "off";
		mode_s = mode == 1 ? "force-on" : mode == 2 ? "force-off" : "auto";
		if (has && ta == tb) {
			snprintf(line, sizeof(line),
				 "temp=%.1f hum=%.1f fan=%s mode=%s high=%.1f low=%.1f",
				 (float)ta / 10.0f, hum, fan_s, mode_s, high, low);
		} else {
			snprintf(line, sizeof(line),
				 "temp=na hum=%.1f fan=%s mode=%s high=%.1f low=%.1f",
				 hum, fan_s, mode_s, high, low);
		}
		{
			int prc = mosquitto_publish(m, NULL, TOPIC_STATUS,
						    (int)strlen(line), line, 0,
						    false);

			/* 发失败就别打成功行：状态没出去却报 [MQTT] status 是假证据 */
			if (prc != MOSQ_ERR_SUCCESS) {
				fprintf(stderr, "[ERR] status publish: %s\n",
					mosquitto_strerror(prc));
				fflush(stderr);
			} else {
				printf("[MQTT] status %s\n", line);
				fflush(stdout);
			}
		}
	} else if (payload_is(msg, "fan on")) {
		state_lock();
		g_st.fan_mode = 1;
		fan_set_unlocked(1);
		state_unlock();
	} else if (payload_is(msg, "fan off")) {
		state_lock();
		g_st.fan_mode = 2;
		fan_set_unlocked(0);
		state_unlock();
	} else if (payload_is(msg, "fan auto")) {
		state_lock();
		g_st.fan_mode = 0;
		state_unlock();
		printf("[MQTT] fan mode auto\n");
		fflush(stdout);
	} else if (msg->payloadlen > 9 &&
		   memcmp(msg->payload, "set high ", 9) == 0) {
		if (payload_number(msg, 9, &v) != 0) {
			fprintf(stderr, "[ERR] bad set high payload (len=%d)\n",
				msg->payloadlen);
			fflush(stderr);
			return;
		}
		state_lock();
		if (v > g_st.t_low)
			g_st.t_high = (float)v;
		state_unlock();
	} else if (msg->payloadlen > 8 &&
		   memcmp(msg->payload, "set low ", 8) == 0) {
		if (payload_number(msg, 8, &v) != 0) {
			fprintf(stderr, "[ERR] bad set low payload (len=%d)\n",
				msg->payloadlen);
			fflush(stderr);
			return;
		}
		state_lock();
		if (v < g_st.t_high)
			g_st.t_low = (float)v;
		state_unlock();
	} else {
		/* 非法载荷一律不改灯/不改风扇/不发 status（含内嵌 NUL 的伪造载荷） */
		fprintf(stderr, "[ERR] unknown command payload (len=%d), ignored\n",
			msg->payloadlen);
		fflush(stderr);
	}
}

static void *thread_comm(void *arg)
{
	(void)arg;
	int rc;

	mosquitto_lib_init();
	mosq = mosquitto_new(resolve_client_id(), true, NULL);
	if (!mosq) {
		fprintf(stderr, "[ERR] mosquitto_new\n");
		atomic_store_explicit(&g_st.running, 0, memory_order_relaxed);
		return NULL;
	}
	mosquitto_connect_callback_set(mosq, on_connect);
	mosquitto_message_callback_set(mosq, on_message);

	{
		const char *bh = getenv("BROKER_HOST");

		if (!bh || !bh[0])
			bh = DEFAULT_BROKER_HOST;
		snprintf(g_broker_host, sizeof(g_broker_host), "%s", bh);
		printf("[MQTT] broker %s:%d USE_LOCK=%d\n", bh, BROKER_PORT,
		       USE_LOCK);
		fflush(stdout);
		rc = mosquitto_connect(mosq, bh, BROKER_PORT, 60);
	}
	if (rc != MOSQ_ERR_SUCCESS) {
		fprintf(stderr, "[ERR] connect: %s\n", mosquitto_strerror(rc));
		/* 无 Broker 时仍允许本地采集/控制演示 */
	}

	{
		int loop_fails = 0;

		while (running_load()) {
			int lrc;

			if (!mosq) {
				usleep(100000);
				continue;
			}

			if (g_sub_failed) {
				/* 订阅失败：当一次断线处理，走下面的重连路径重订 */
				g_sub_failed = 0;
				lrc = MOSQ_ERR_CONN_LOST;
			} else {
				lrc = mosquitto_loop(mosq, 100, 1);
			}
			if (lrc == MOSQ_ERR_SUCCESS) {
				loop_fails = 0;
				continue;
			}

			if (lrc == MOSQ_ERR_CONN_LOST || lrc == MOSQ_ERR_NO_CONN ||
			    lrc == MOSQ_ERR_CONN_REFUSED ||
			    lrc == MOSQ_ERR_PROTOCOL) {
				/* 断线类错误：重连，连续失败到上限就收摊，别空转 */
				loop_fails++;
				fprintf(stderr, "[ERR] mqtt loop: %s (retry %d/%d)\n",
					mosquitto_strerror(lrc), loop_fails,
					MQTT_MAX_RETRY);
				fflush(stderr);
				if (loop_fails >= MQTT_MAX_RETRY) {
					/*
					 * 只收 MQTT 这一条腿：采集/控制线程继续跑本地
					 * 演示，与上面 connect 失败那句注释一致。以前
					 * 这里置 running=0，把整个程序一起带走了。
					 */
					fprintf(stderr,
						"[ERR] broker unreachable, MQTT disabled; "
						"sense/control keep running (Ctrl+C to stop)\n");
					fflush(stderr);
					break;
				}
				if (mqtt_reconnect() != 0)
					fprintf(stderr, "[ERR] reconnect failed\n");
				usleep(500000);
				continue;
			}

			/* 其它错误：记一行、短暂退避，避免刷屏空转 */
			fprintf(stderr, "[ERR] mqtt loop: %s\n",
				mosquitto_strerror(lrc));
			fflush(stderr);
			usleep(200000);
		}
	}

	if (mosq) {
		mosquitto_disconnect(mosq);
		mosquitto_destroy(mosq);
		mosq = NULL;
	}
	mosquitto_lib_cleanup();
	return NULL;
}

int main(void)
{
	pthread_t th_s, th_c, th_m;

	memset(&g_st, 0, sizeof(g_st));
	g_st.t_high = 28.0f;
	g_st.t_low = 26.0f;
	atomic_init(&g_st.running, 1);
	pthread_mutex_init(&g_st.lock, NULL);

	signal(SIGINT, on_sigint);

	chip = gpiod_chip_open(GPIO_CHIP_PATH);
	if (!chip) {
		perror("gpiod_chip_open");
		return 1;
	}
	if (fan_init() < 0) {
		perror("fan_init");
		gpiod_chip_close(chip);
		return 1;
	}

	printf("[INFO] USE_LOCK=%d SIMULATE_SENSOR=%d\n", USE_LOCK, SIMULATE_SENSOR);
	printf("[INFO] Ctrl+C for clean stop\n");

	{
		int s_ok, c_ok, m_ok;

		/* 起不全就整体收摊：join 未创建的 pthread_t 是未定义行为 */
		s_ok = pthread_create(&th_s, NULL, thread_sense, NULL) == 0;
		if (!s_ok)
			perror("pthread_create sense");
		c_ok = s_ok &&
		       pthread_create(&th_c, NULL, thread_control, NULL) == 0;
		if (s_ok && !c_ok)
			perror("pthread_create control");
		m_ok = c_ok && pthread_create(&th_m, NULL, thread_comm, NULL) == 0;
		if (c_ok && !m_ok)
			perror("pthread_create comm");
		if (!m_ok) {
			fprintf(stderr, "[ERR] thread create failed, stopping\n");
			fflush(stderr);
			atomic_store_explicit(&g_st.running, 0,
					      memory_order_relaxed);
		}

		if (m_ok)
			pthread_join(th_m, NULL);
		if (c_ok)
			pthread_join(th_c, NULL);
		if (s_ok)
			pthread_join(th_s, NULL);
	}

	/* 干净停：关风扇（写入失败要报错，不假装已关） */
	state_lock();
	if (fan_set_unlocked(0) < 0)
		fprintf(stderr, "[ERR] cleanup: fan OFF write failed\n");
	state_unlock();

	if (fan_req)
		gpiod_line_request_release(fan_req);
	gpiod_chip_close(chip);
	pthread_mutex_destroy(&g_st.lock);
	printf("[INFO] race_hits=%d (USE_LOCK=%d；期望 0→很多，1→0)\n",
	       g_st.race_hits, USE_LOCK);
	printf("[INFO] exited cleanly\n");
	return 0;
}
