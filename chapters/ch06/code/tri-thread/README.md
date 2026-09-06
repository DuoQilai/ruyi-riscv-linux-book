# tri-thread · 三线程协同骨架

荔枝派 4A：采集 ∥ 控制 ∥ 通信(MQTT)。供本章实验与综合项目复用（滞回一直跑）。

- `USE_LOCK 0`：故意不加锁，观察共享数据错乱  
- `USE_LOCK 1`：加锁后行为应正确  
- Ctrl+C：`running=0`，关风扇后退出  

板端原生 `make`；交叉：先激活 `venv-gnu-ruyisdk`，再 `make CROSS_COMPILE=riscv64-ruyisdk-linux-gnu-`。依赖 libpthread、libmosquitto、libgpiod。

## 构建与运行

```bash
# 板端
make
BROKER_HOST=<主机局域网IP> ./tri-thread

# 或主机交叉后 scp
make CROSS_COMPILE=riscv64-ruyisdk-linux-gnu-
scp tri-thread user@board-ip:~/
```

Broker 用环境变量 `BROKER_HOST`（优先于源码默认值）。可选 `MQTT_CLIENT_ID`；默认 `tri-thread-<主机名>`。

主题：`course/thermo/cmd` / `course/thermo/status`。GPIO：风扇 `IO1_5`、传感器 `IO1_6`。线程循环内标 TODO 的部分需按讲义补全。
