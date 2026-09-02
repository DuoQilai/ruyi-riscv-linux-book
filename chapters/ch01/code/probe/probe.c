/*
 * probe.c — 第一章最小程序（实验一 / 实验二共用）
 *
 * 打印两件事：
 *   1) 编译时刻的架构宏：__riscv 有定义 → riscv；否则本机（通常 x86-64）
 *   2) 指针宽度 sizeof(void*)
 *
 * 同一份源码，在哪台机器 gcc，产物就是哪台机器的架构：
 *   板端(riscv64)  make → RISC-V ELF，能跑
 *   主机(x86-64)   make → x86-64 ELF，上板会 Exec format error
 *
 * 用 file / readelf 认产物架构，是本章练习的一部分。
 */
#include <stdio.h>

int main(void)
{
#if defined(__riscv)
	printf("arch     = riscv\n");
#else
	printf("arch     = x86-64 (非 RISC-V，本机 gcc 产物)\n");
#endif
	printf("ptr size = %zu bytes\n", sizeof(void *));
	printf("probe done.\n");
	return 0;
}
