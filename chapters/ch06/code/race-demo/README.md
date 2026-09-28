# race-demo · 第六章实验一

两线程无锁抢同一计数器。先看丢更新，再用 TSan 指出抢变量的两行。

```bash
make && ./race-demo                    # 看丢更新（got < expect）
sudo sysctl vm.mmap_rnd_bits=28        # 主机 TSan 若启动失败再执行
make tsan && ./race-demo               # ThreadSanitizer 报告
```

不要把无锁写法抄进实验二 `snapshot-lock` / 实验三 `tri-thread` 的验收路径。
