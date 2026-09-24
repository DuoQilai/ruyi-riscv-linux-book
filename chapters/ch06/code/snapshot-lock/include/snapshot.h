/*
 * snapshot.h — 第六章实验二：成对快照 + 互斥锁（纯软件）
 *
 * 实验三 tri-thread 里「同一次温度拆成 temp_a/temp_b」的缩微版。
 * 你实现 pair_write / pair_read：无锁必现不一致；加锁后 race_hits→0。
 */
#ifndef SNAPSHOT_H
#define SNAPSHOT_H

void pair_init(void);
void pair_destroy(void);

/* 把同一次采样值 v 写入 temp_a 与 temp_b（应成对一致） */
void pair_write(int v);

/* 读出一对；若 a!=b 返回 1（不一致），否则 0 */
int pair_read(int *a, int *b);

#endif
