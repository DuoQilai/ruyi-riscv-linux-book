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
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define USE_LOCK         1 /* 实验：先改 0 看错乱，再改回 1 */
#define SIMULATE_SENSOR  1

#define BROKER_HOST      "192.168.1.10" /* TODO */
#define BROKER_PORT      1883
#define TOPIC_CMD        "course/thermo/cmd"
#define TOPIC_STATUS     "course/thermo/status"
#define CLIENT_ID        "licheepi4a-tri-thread"

#define GPIO_CHIP_PATH   "/dev/gpiochip5"
#define DHT_LINE         6  /* IO1_6 */
#define FAN_LINE         5  /* IO1_5 */
#define SAMPLE_MS        500
#define CONTROL_MS       200

struct shared_state {
	pthread_mutex_t lock;
	float last_temp;
	float last_hum;
	float t_high;
	float t_low;
	int fan_on;
	int has_sample;
	volatile int running;
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

	if (!s || !lc || !rc)
		return NULL;
	gpiod_line_settings_set_direction(s, direction);
	gpiod_line_config_add_line_settings(lc, offs, 1, s);
	gpiod_request_config_set_consumer(rc, "tri-thread");

	r = gpiod_chip_request_lines(chip, rc, lc);
	gpiod_request_config_free(rc);
	gpiod_line_config_free(lc);
	gpiod_line_settings_free(s);

	if (r && direction == GPIOD_LINE_DIRECTION_OUTPUT)
		gpiod_line_request_set_value(r, offset, val);
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
	g_st.running = 0;
}

static int fan_init(void)
{
	fan_req = line_request(FAN_LINE, GPIOD_LINE_DIRECTION_OUTPUT, 0);
	if (!fan_req)
		return -1;
	return 0;
}

static void fan_set_unlocked(int on)
{
	if (!fan_req)
		return;
	gpiod_line_request_set_value(fan_req, FAN_LINE, on ? 1 : 0);
	g_st.fan_on = on;
	printf("[INFO] fan %s\n", on ? "ON" : "OFF");
	fflush(stdout);
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
	while (g_st.running) {
		float t = 0, h = 0;
		if (dht22_read(&t, &h) == 0) {
			state_lock();
			g_st.last_temp = t;
			g_st.last_hum = h;
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
	while (g_st.running) {
		float t, high, low;
		int fan, has;

		state_lock();
		t = g_st.last_temp;
		high = g_st.t_high;
		low = g_st.t_low;
		fan = g_st.fan_on;
		has = g_st.has_sample;
		state_unlock();

		if (has) {
			if (t > high && !fan) {
				state_lock();
				fan_set_unlocked(1);
				state_unlock();
			} else if (t < low && fan) {
				state_lock();
				fan_set_unlocked(0);
				state_unlock();
			}
		}
		usleep((useconds_t)CONTROL_MS * 1000);
	}
	return NULL;
}

/* ---------- 通信线程（MQTT） ---------- */
static void on_connect(struct mosquitto *m, void *obj, int rc)
{
	(void)obj;
	if (rc != 0)
		return;
	mosquitto_subscribe(m, NULL, TOPIC_CMD, 0);
	printf("[MQTT] subscribed %s\n", TOPIC_CMD);
	fflush(stdout);
}

static void on_message(struct mosquitto *m, void *obj,
		       const struct mosquitto_message *msg)
{
	(void)m;
	(void)obj;
	char buf[64];
	float v;

	if (!msg || !msg->payload)
		return;
	snprintf(buf, sizeof(buf), "%.*s",
		 msg->payloadlen > 63 ? 63 : msg->payloadlen,
		 (const char *)msg->payload);
	printf("[MQTT] cmd payload=%s\n", buf);
	fflush(stdout);

	/*
	 * TODO: 解析简单命令，例如：
	 *   set high 30
	 *   set low 25
	 *   status   → 读共享状态并 mosquitto_publish 到 TOPIC_STATUS
	 */
	if (sscanf(buf, "set high %f", &v) == 1) {
		state_lock();
		if (v > g_st.t_low)
			g_st.t_high = v;
		state_unlock();
	} else if (sscanf(buf, "set low %f", &v) == 1) {
		state_lock();
		if (v < g_st.t_high)
			g_st.t_low = v;
		state_unlock();
	}
}

static void *thread_comm(void *arg)
{
	(void)arg;
	int rc;

	mosquitto_lib_init();
	mosq = mosquitto_new(CLIENT_ID, true, NULL);
	if (!mosq) {
		fprintf(stderr, "[ERR] mosquitto_new\n");
		g_st.running = 0;
		return NULL;
	}
	mosquitto_connect_callback_set(mosq, on_connect);
	mosquitto_message_callback_set(mosq, on_message);

	rc = mosquitto_connect(mosq, BROKER_HOST, BROKER_PORT, 60);
	if (rc != MOSQ_ERR_SUCCESS) {
		fprintf(stderr, "[ERR] connect: %s\n", mosquitto_strerror(rc));
		/* 无 Broker 时仍允许本地采集/控制演示 */
	}

	while (g_st.running) {
		if (mosq)
			mosquitto_loop(mosq, 100, 1);
		else
			usleep(100000);
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
	g_st.running = 1;
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

	pthread_create(&th_s, NULL, thread_sense, NULL);
	pthread_create(&th_c, NULL, thread_control, NULL);
	pthread_create(&th_m, NULL, thread_comm, NULL);

	pthread_join(th_s, NULL);
	pthread_join(th_c, NULL);
	pthread_join(th_m, NULL);

	/* 干净停：关风扇 */
	state_lock();
	fan_set_unlocked(0);
	state_unlock();

	if (fan_req)
		gpiod_line_request_release(fan_req);
	gpiod_chip_close(chip);
	pthread_mutex_destroy(&g_st.lock);
	printf("[INFO] exited cleanly\n");
	return 0;
}
