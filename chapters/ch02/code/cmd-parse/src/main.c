/*
 * main.c — cmd-parse 测试驱动（实验二）
 * make && ./build/cmd-parse；每个样例打印切词结果，最后 ALL PASS。
 * 别改本文件；只实现 src/parse.c 里的 split_tokens。
 */
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "parse.h"

struct sample {
	const char *line;
	int         exp_n;   /* 期望词数 */
	double      num;     /* 期望出现的数字值；没有则 0（且 expect_num=0） */
	int         expect_num;
};

static const struct sample SAMPLES[] = {
	{ "set temp 28.5",           3,  28.5, 1 },
	{ "status",                  1,   0.0, 0 },
	{ "set humidity 60",         3,  60.0, 1 },
	{ "   set  speed  3.25  ",   3,   3.25, 1 },
	{ "",                        0,   0.0, 0 },
	{ "set temp abc",            3,   0.0, 0 }, /* abc 不是数字 */
};

static void dump(const Tok *t, int n)
{
	int i;
	for (i = 0; i < n; i++) {
		if (t[i].is_num)
			printf("        '%s' = %.2f\n", t[i].word, t[i].val);
		else
			printf("        '%s'\n", t[i].word);
	}
}

int main(void)
{
	size_t i;
	int fails = 0;

	for (i = 0; i < sizeof(SAMPLES) / sizeof(SAMPLES[0]); i++) {
		Tok toks[8];
		int n = split_tokens(SAMPLES[i].line, toks, 8);
		int has_num = 0, j;
		double got = 0.0;

		printf("== \"%s\" -> %d 词\n", SAMPLES[i].line, n);
		dump(toks, n);

		for (j = 0; j < n && j < 8; j++) {
			if (toks[j].is_num) {
				has_num = 1;
				got = toks[j].val;
			}
		}

		if (n != SAMPLES[i].exp_n) {
			printf("    FAIL 词数 %d != 期望 %d\n", n, SAMPLES[i].exp_n);
			fails++;
		} else if (SAMPLES[i].expect_num && (!has_num || fabs(got - SAMPLES[i].num) > 0.001)) {
			printf("    FAIL 数字值 %.2f != 期望 %.2f\n", got, SAMPLES[i].num);
			fails++;
		} else if (!SAMPLES[i].expect_num && has_num && strcmp(SAMPLES[i].line, "set temp abc") == 0) {
			/* abc 这一行不应出现数字词 */
			printf("    FAIL 'abc' 不应被当成数字\n");
			fails++;
		} else {
			printf("    ok\n");
		}
	}

	if (fails) {
		printf("\nSOME FAIL (%d)\n", fails);
		return 1;
	}
	printf("\nALL PASS\n");
	return 0;
}
