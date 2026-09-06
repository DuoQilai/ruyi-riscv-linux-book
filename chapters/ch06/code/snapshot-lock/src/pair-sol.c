/*
 * pair-sol.c — 参考实现：同一把 mutex 保护成对写/读，无空窗。
 */
#include <pthread.h>
#include <unistd.h>

#include "snapshot.h"

static pthread_mutex_t g_lock = PTHREAD_MUTEX_INITIALIZER;
static int g_temp_a;
static int g_temp_b;

void pair_init(void)
{
	g_temp_a = 0;
	g_temp_b = 0;
}

void pair_destroy(void)
{
}

void pair_write(int v)
{
	pthread_mutex_lock(&g_lock);
	g_temp_a = v;
	g_temp_b = v;
	pthread_mutex_unlock(&g_lock);
}

int pair_read(int *a, int *b)
{
	int raced;

	pthread_mutex_lock(&g_lock);
	*a = g_temp_a;
	*b = g_temp_b;
	raced = (*a != *b) ? 1 : 0;
	pthread_mutex_unlock(&g_lock);
	return raced;
}
