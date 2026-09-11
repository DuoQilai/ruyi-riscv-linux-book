/*
 * main.c — snapshot-lock 驱动（实验二）。
 * 写线程成对写递增温度；读线程抓快照并统计不一致次数。
 * g_running 用 C11 原子，避免读/写线程对退出标志的数据竞争（TSan）。
 */
#include <pthread.h>
#include <stdatomic.h>
#include <stdio.h>
#include <unistd.h>

#include "snapshot.h"

#define ROUNDS 40

static atomic_int g_running = 1;

static void *writer(void *arg)
{
	int i;
	(void)arg;
	for (i = 0; i < ROUNDS; i++) {
		pair_write(250 + i); /* 25.0℃ 起，0.1℃ 步长的整数表示 */
		usleep(30000);
	}
	atomic_store_explicit(&g_running, 0, memory_order_release);
	return NULL;
}

static void *reader(void *arg)
{
	int hits = 0;
	int drain;

	(void)arg;
	while (atomic_load_explicit(&g_running, memory_order_acquire)) {
		int a = 0, b = 0;
		if (pair_read(&a, &b)) {
			hits++;
			printf("[RACE] inconsistent snapshot temp_a=%d temp_b=%d\n",
			       a, b);
			fflush(stdout);
		}
		usleep(5000);
	}
	/* 写线程停后多读几轮，避免漏掉尾部空窗 */
	for (drain = 0; drain < 8; drain++) {
		int a = 0, b = 0;
		if (pair_read(&a, &b)) {
			hits++;
			printf("[RACE] inconsistent snapshot temp_a=%d temp_b=%d\n",
			       a, b);
			fflush(stdout);
		}
		usleep(5000);
	}
	*(int *)arg = hits;
	return NULL;
}

int main(void)
{
	pthread_t tw, tr;
	int race_hits = 0;

	pair_init();
	printf("[S] snapshot-lock: writer∥reader；看 [RACE] 与 race_hits\n");
	fflush(stdout);

	pthread_create(&tr, NULL, reader, &race_hits);
	pthread_create(&tw, NULL, writer, NULL);
	pthread_join(tw, NULL);
	pthread_join(tr, NULL);

	printf("[INFO] race_hits=%d（脚手架无锁→很多；make sol 加锁→0）\n",
	       race_hits);
	fflush(stdout);
	pair_destroy();
	return race_hits > 0 ? 1 : 0;
}
