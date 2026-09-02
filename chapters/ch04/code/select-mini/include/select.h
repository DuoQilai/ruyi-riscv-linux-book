/*
 * select.h — select 最小多路复用（实验一素材）
 * 用 select 同时等两件事：stdin 有输入 OR 定时 tick 到期。
 */
#ifndef SELECT_H
#define SELECT_H

/*
 * 跑一个 select 主循环，同时等待 fd 上的输入与 tick_s 周期的定时：
 *   - 超时（无输入）：打印 [tick N]，继续；
 *   - fd 可读：读一行；
 *       "quit" → 打印 bye 并返回 0；
 *       EOF   → 打印 [EOF] 并返回 0；
 *       其它   → 打印 echo: <行>，继续。
 * 出错返回 1。
 */
int run_repl_loop(int fd, double tick_s);

#endif
