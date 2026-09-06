# race-demo · 第六章实验一

两线程无锁对同一计数器做「读→加一→写」。期望值 = `2 * N`，无锁时结果偏小。主机或板端均可。

```bash
make && ./race-demo                    # 看丢更新（got < expect）
# 主机若 TSan 启动失败：sudo sysctl vm.mmap_rnd_bits=28
make tsan && ./race-demo               # ThreadSanitizer 报告抢变量的行
```

不要把无锁写法抄进实验二 `tri-thread`。
