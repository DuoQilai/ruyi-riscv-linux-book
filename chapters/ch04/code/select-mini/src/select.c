/*
 * select.c — select 主循环（学生版）
 * 只实现 run_repl_loop。对照答案：make sol && ./build/select-mini-sol
 */
#include <stdio.h>
#include <string.h>
#include <sys/select.h>
#include <unistd.h>
#include "select.h"

/* STUDENT TODO [S1]：用 select 同时等「输入」和「1 秒超时」。
 *
 * 骨架建议：
 *   for (;;) {
 *     FD_ZERO + FD_SET(fd)；
 *     把 tick_s 换算成 struct timeval；
 *     r = select(fd+1, &rfds, NULL, NULL, &tv)；
 *     r<0 → perror 返回 1；
 *     r==0 → 打 [tick N] 后 continue；
 *     fd 可读 → fgets 一行，quit→bye 返回 0，EOF→[EOF] 返回 0，否则 echo。
 *   }
 */
int run_repl_loop(int fd, double tick_s)
{
	printf("[S] select demo: %.0fs tick, stdin echo, quit to exit\n", tick_s);
	return 0;   /* ← 这里实现 */
}
