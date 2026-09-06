/*
 * ch05 mqtt-led — 第五章实验三：MQTT 远程控灯（主实验脚手架）
 *
 * 下行：订阅 TOPIC_CMD，payload on/off → GPIO LED
 * 上行：publish_status() 发到 TOPIC_STATUS（学生补全）
 *
 * 板：荔枝派 4A + RevyOS。libgpiod v2 API。依赖：libmosquitto、libgpiod。
 * 脚位：外接 LED 接 IO1_4（gpiochip5 line 4）。
 * IO1_5 / IO1_6 留给风扇与 DHT。板上实测 IO1_3 拉高读回异常，默认不用 3。
 *
 * 编译（板端原生）：make ；交叉：make CROSS_COMPILE=riscv64-ruyisdk-linux-gnu-
 */
#include <gpiod.h>
#include <mosquitto.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* 默认占位；运行前优先用环境变量 BROKER_HOST（见 lab / README） */
#define DEFAULT_BROKER_HOST "192.168.1.10"
#define BROKER_PORT         1883
#define TOPIC_CMD           "course/led/cmd"
#define TOPIC_STATUS        "course/led/status"
#define DEFAULT_CLIENT_ID   "licheepi4a-mqtt-led"

#define GPIO_CHIP_PATH   "/dev/gpiochip5"
#define LED_LINE         4 /* IO1_4 */

static struct gpiod_chip *chip;
static struct gpiod_line_request *led_req;
static struct mosquitto *mosq;
static int led_on;
static char client_id_buf[128];

/* MQTT_CLIENT_ID 优先；否则 mqtt-led-<hostname>，避免多板同 ID 互踢 */
static const char *resolve_client_id(void)
{
	const char *env = getenv("MQTT_CLIENT_ID");
	char host[64];

	if (env && env[0])
		return env;
	if (gethostname(host, sizeof(host)) == 0 && host[0]) {
		snprintf(client_id_buf, sizeof(client_id_buf),
			 "mqtt-led-%s", host);
		return client_id_buf;
	}
	return DEFAULT_CLIENT_ID;
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

/* libgpiod v2：申请一根输出线，初始置 val */
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
	gpiod_request_config_set_consumer(rc, "mqtt-led");

	r = gpiod_chip_request_lines(chip, rc, lc);
	gpiod_request_config_free(rc);
	gpiod_line_config_free(lc);
	gpiod_line_settings_free(s);

	if (r && direction == GPIOD_LINE_DIRECTION_OUTPUT)
		gpiod_line_request_set_value(r, offset, val);
	return r;
}

static int led_init(void)
{
	led_req = line_request(LED_LINE, GPIOD_LINE_DIRECTION_OUTPUT, 0);
	if (!led_req) {
		perror("led request_output");
		return -1;
	}
	led_on = 0;
	return 0;
}

/* 成功返回 0；失败返回 -1（调用方不要发 status） */
static int led_set(int on)
{
	if (!led_req)
		return -1;
	if (gpiod_line_request_set_value(led_req, LED_LINE, on ? 1 : 0) < 0) {
		perror("led set_value");
		return -1;
	}
	led_on = on;
	printf("[INFO] LED %s\n", on ? "ON" : "OFF");
	fflush(stdout);
	return 0;
}

/*
 * TODO: 用 mosquitto_publish 向 TOPIC_STATUS 发布 payload（如 "on"/"off"）。
 * 成功打 [INFO]，失败打 [ERR]。
 */
static void publish_status(const char *payload)
{
	(void)payload;
	(void)mosq;
	printf("[TODO] publish_status(\"%s\") not implemented\n",
	       payload ? payload : "");
	fflush(stdout);
}

/* 连接成功后订阅命令主题 */
static void on_connect(struct mosquitto *m, void *obj, int rc)
{
	(void)obj;
	if (rc != 0) {
		fprintf(stderr, "[ERR] connect failed rc=%d\n", rc);
		return;
	}
	log_info("connected to broker");
	if (mosquitto_subscribe(m, NULL, TOPIC_CMD, 0) != MOSQ_ERR_SUCCESS) {
		log_err("subscribe failed");
		mosquitto_disconnect(m);
		return;
	}
	printf("[INFO] subscribed %s\n", TOPIC_CMD);
}

/*
 * TODO: 根据 payload 调用 led_set，成功后再 publish_status。
 * 约定：精确匹配 "on" / "off"；其他打印 [ERR] 且不改灯。
 * led_set 失败时不要发 status。
 */
static void on_message(struct mosquitto *m, void *obj,
		       const struct mosquitto_message *msg)
{
	(void)m;
	(void)obj;
	char buf[64];

	if (!msg || !msg->topic)
		return;
	snprintf(buf, sizeof(buf), "%.*s",
		 msg->payloadlen > 63 ? 63 : msg->payloadlen,
		 msg->payload ? (const char *)msg->payload : "");
	printf("[INFO] msg topic=%s payload=%s\n", msg->topic, buf);
	fflush(stdout);

	/* TODO: 解析 on/off → led_set → publish_status */
	printf("[TODO] handle cmd payload in on_message\n");
	fflush(stdout);
	(void)led_on;
}

int main(void)
{
	int rc;
	const char *broker_host = getenv("BROKER_HOST");
	const char *client_id = resolve_client_id();

	if (!broker_host || !broker_host[0])
		broker_host = DEFAULT_BROKER_HOST;

	chip = gpiod_chip_open(GPIO_CHIP_PATH);
	if (!chip) {
		perror("gpiod_chip_open");
		return 1;
	}
	if (led_init() < 0) {
		gpiod_chip_close(chip);
		return 1;
	}

	mosquitto_lib_init();
	mosq = mosquitto_new(client_id, true, NULL);
	if (!mosq) {
		log_err("mosquitto_new failed");
		gpiod_chip_close(chip);
		return 1;
	}
	printf("[INFO] client_id=%s\n", client_id);
	fflush(stdout);
	mosquitto_connect_callback_set(mosq, on_connect);
	mosquitto_message_callback_set(mosq, on_message);

	printf("[INFO] connecting %s:%d ...\n", broker_host, BROKER_PORT);
	rc = mosquitto_connect(mosq, broker_host, BROKER_PORT, 60);
	if (rc != MOSQ_ERR_SUCCESS) {
		fprintf(stderr, "[ERR] mosquitto_connect: %s\n",
			mosquitto_strerror(rc));
		mosquitto_destroy(mosq);
		mosquitto_lib_cleanup();
		gpiod_chip_close(chip);
		return 1;
	}

	log_info("loop start (Ctrl+C to stop)");
	/* 阻塞循环；综合项目可改为 mosquitto_loop_start 线程模式 */
	mosquitto_loop_forever(mosq, -1, 1);

	mosquitto_destroy(mosq);
	mosquitto_lib_cleanup();
	if (led_req)
		gpiod_line_request_release(led_req);
	gpiod_chip_close(chip);
	return 0;
}
