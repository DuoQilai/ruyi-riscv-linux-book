/*
 * main.c — select-mini 驱动（实验一）
 * 每 1 秒打一个 tick，同时回应 stdin 里的行。别改本文件，只实现 select.c。
 *
 * make && ./build/select-mini
 *   然后手动敲几行（quit 退出），或自动演示：
 *   (sleep 3; echo hello; sleep 1; echo quit) | ./build/select-mini
 */
#include <stdio.h>
#include <unistd.h>
#include "select.h"

int main(void)
{
	return run_repl_loop(STDIN_FILENO, 1.0);
}
