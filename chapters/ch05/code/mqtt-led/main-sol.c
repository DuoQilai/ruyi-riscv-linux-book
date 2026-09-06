/*
 * mqtt-led-sol.c — 第五章参考实现（验证用）
 * 学生版见 main.c（保留 TODO）。脚位 IO1_4；Broker 填主机局域网 IP。
 */
#include <gpiod.h>
#include <mosquitto.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* 默认占位；优先读环境变量 BROKER_HOST */
#define DEFAULT_BROKER_HOST "192.168.1.10"
#define BROKER_PORT         1883
#define TOPIC_CMD           "course/led/cmd"
#define TOPIC_STATUS        "course/led/status"
#define CLIENT_ID           "licheepi4a-mqtt-led"

#define GPIO_CHIP_PATH   "/dev/gpiochip5"
#define LED_LINE         4 /* IO1_4 */

static struct gpiod_chip *chip;
static struct gpiod_line_request *led_req;
static struct mosquitto *mosq;
static int led_on;

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

static void led_set(int on)
{
	if (!led_req)
		return;
	if (gpiod_line_request_set_value(led_req, LED_LINE, on ? 1 : 0) < 0) {
		perror("led set_value");
		return;
	}
	led_on = on;
	printf("[INFO] LED %s\n", on ? "ON" : "OFF");
	fflush(stdout);
}

static void publish_status(const char *payload)
{
	int rc;

	if (!mosq || !payload)
		return;
	rc = mosquitto_publish(mosq, NULL, TOPIC_STATUS,
			       (int)strlen(payload), payload, 0, false);
	if (rc != MOSQ_ERR_SUCCESS) {
		fprintf(stderr, "[ERR] publish_status: %s\n",
			mosquitto_strerror(rc));
		fflush(stderr);
		return;
	}
	printf("[INFO] published %s = %s\n", TOPIC_STATUS, payload);
	fflush(stdout);
}

static void on_connect(struct mosquitto *m, void *obj, int rc)
{
	(void)obj;
	if (rc != 0) {
		fprintf(stderr, "[ERR] connect failed rc=%d\n", rc);
		return;
	}
	log_info("connected to broker");
	if (mosquitto_subscribe(m, NULL, TOPIC_CMD, 0) != MOSQ_ERR_SUCCESS)
		log_err("subscribe failed");
	else
		printf("[INFO] subscribed %s\n", TOPIC_CMD);
}

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

	if (strcmp(buf, "on") == 0) {
		led_set(1);
		publish_status("on");
	} else if (strcmp(buf, "off") == 0) {
		led_set(0);
		publish_status("off");
	} else {
		fprintf(stderr, "[ERR] unknown payload (want on/off): %s\n",
			buf);
		fflush(stderr);
	}
}

int main(void)
{
	int rc;
	const char *broker_host = getenv("BROKER_HOST");

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
	mosq = mosquitto_new(CLIENT_ID, true, NULL);
	if (!mosq) {
		log_err("mosquitto_new failed");
		gpiod_chip_close(chip);
		return 1;
	}
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
	mosquitto_loop_forever(mosq, -1, 1);

	mosquitto_destroy(mosq);
	mosquitto_lib_cleanup();
	if (led_req)
		gpiod_line_request_release(led_req);
	gpiod_chip_close(chip);
	return 0;
}
