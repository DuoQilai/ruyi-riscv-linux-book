/*
 * ch06 race-demo — 确定性强制数据竞争（电脑本机跑）
 *
 * 两线程无锁对同一 counter 做「读 → 加一 → 写」；中间无原子、无锁。
 * 期望最终值 = 2*N；无锁时几乎必小于期望（丢更新）。
 * 用 make tsan 编译后，ThreadSanitizer 会指出抢变量的两行。
 *
 * 勿上板；勿把无锁写法用于产品代码。
 */
#include <pthread.h>
#include <stdio.h>
#include <stdint.h>

#define N 200000u

/* 故意无锁的共享计数器（volatile 防止编译器把循环优化没） */
static volatile uint64_t counter;

/* 非原子读改写：两线程交错时会丢更新 */
static void bump_racy(void)
{
	uint64_t tmp;

	tmp = counter; /* 竞争读 */
	tmp = tmp + 1;
	counter = tmp; /* 竞争写 */
}

static void *worker(void *arg)
{
	uint32_t i;
	(void)arg;
	for (i = 0; i < N; i++)
		bump_racy();
	return NULL;
}

int main(void)
{
	pthread_t a, b;
	uint64_t expect = (uint64_t)N * 2u;

	counter = 0;
	pthread_create(&a, NULL, worker, NULL);
	pthread_create(&b, NULL, worker, NULL);
	pthread_join(a, NULL);
	pthread_join(b, NULL);

	printf("expect=%llu  got=%llu  %s\n",
	       (unsigned long long)expect,
	       (unsigned long long)counter,
	       counter == expect ? "UNEXPECTED_FULL" : "RACE(lost updates)");
	/* 无锁路径期望「不等于」；若偶发相等再跑一次即可 */
	return counter == expect ? 0 : 1;
}
