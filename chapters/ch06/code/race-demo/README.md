# race-demo · 强制数据竞争（电脑演示）

两线程无锁对同一计数器做「读→加一→写」。期望值 = `2 * N`，无锁时结果偏小。

```bash
make && ./race-demo                    # 看丢更新（got < expect）
sudo sysctl vm.mmap_rnd_bits=28        # 固定步骤：让 TSan 稳定跑通
make tsan && ./race-demo               # ThreadSanitizer 报告抢变量的行
```

只用主机 `gcc`，不上板。不要把无锁写法抄进 `tri-thread`。
