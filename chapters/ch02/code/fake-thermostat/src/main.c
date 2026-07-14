/*
 * main.c — 假温控驱动：内置温度序列或 stdin
 * 软件输入，无真 DHT。半成品逻辑在 thermo.c，请按关卡修复。
 */
#include "thermo.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
	float temp_c;
	int   fail; /* 1 = 本轮模拟读失败 */
} Sample;

/* 内置序列：最后一笔 fail=1 用于验收 L4 */
static const Sample k_builtin[] = {
	{ 25.0f, 0 },
	{ 27.0f, 0 },
	{ 29.0f, 0 },
	{ 27.0f, 0 },
	{ 24.0f, 0 },
	{ 0.0f,  1 }, /* 哨兵：读失败 */
};

static void print_decision(int step, float temp_c, int ok, int fan_on)
{
	if (!ok) {
		printf("step %d: READ_FAIL  fan=%s (hold)\n",
		       step, fan_on ? "ON" : "OFF");
		return;
	}
	printf("step %d: temp=%.1f  fan=%s\n",
	       step, temp_c, fan_on ? "ON" : "OFF");
}

static int run_samples(const ThermoConfig *cfg, const Sample *samples, size_t n)
{
	int fan_on = 0;
	size_t i;

	for (i = 0; i < n; i++) {
		float t = 0.0f;
		int rc;

		thermo_inject(samples[i].temp_c, samples[i].fail);
		rc = thermo_read(&t);
		if (rc != 0) {
			/* 读失败：跳过决策，保持 fan_on */
			print_decision((int)i + 1, t, 0, fan_on);
			continue;
		}
		thermo_decide(cfg, t, &fan_on);
		print_decision((int)i + 1, t, 1, fan_on);
	}
	return 0;
}

static int run_stdin(const ThermoConfig *cfg)
{
	int fan_on = 0;
	int step = 0;
	char line[128];

	while (fgets(line, sizeof(line), stdin)) {
		float t;
		int rc;
		char *end = NULL;

		if (line[0] == '\n' || line[0] == '#')
			continue;

		t = strtof(line, &end);
		if (end == line)
			continue;

		step++;
		/* 约定：负数表示本轮读失败 */
		if (t < 0.0f) {
			thermo_inject(t, 1);
		} else {
			thermo_inject(t, 0);
		}

		rc = thermo_read(&t);
		if (rc != 0) {
			print_decision(step, t, 0, fan_on);
			continue;
		}
		thermo_decide(cfg, t, &fan_on);
		print_decision(step, t, 1, fan_on);
	}
	return 0;
}

int main(int argc, char **argv)
{
	ThermoConfig cfg = {
		.t_high = 28.0f,
		.t_low = 26.0f,
		.period_ms = 2000,
	};
	int use_stdin = 0;
	int i;

	for (i = 1; i < argc; i++) {
		if (strcmp(argv[i], "--stdin") == 0)
			use_stdin = 1;
	}

	printf("fake-thermostat  t_high=%.1f  t_low=%.1f  period_ms=%d\n",
	       cfg.t_high, cfg.t_low, cfg.period_ms);

	if (use_stdin)
		return run_stdin(&cfg);

	return run_samples(&cfg, k_builtin, sizeof(k_builtin) / sizeof(k_builtin[0]));
}
