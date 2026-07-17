# tri-thread · 三线程协同骨架

荔枝派 4A：采集 ∥ 控制 ∥ 通信(MQTT)。供本章实验与综合项目复用。

- `USE_LOCK 0`：故意不加锁，观察共享数据错乱  
- `USE_LOCK 1`：加锁后行为应正确  
- Ctrl+C：`running=0`，关风扇后退出  

交叉前缀默认 `riscv64-unknown-linux-gnu-`（RuyiSDK）。依赖 libpthread、libmosquitto、libgpiod。

## 构建

```bash
make
scp tri-thread user@board-ip:~/
ssh user@board-ip './tri-thread'
```

先改 `BROKER_HOST` 与 GPIO 宏。线程循环内标 TODO 的部分需按讲义补全。
