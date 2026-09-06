# tri-thread · 第六章实验三（主实验）

荔枝派 4A：采集 ∥ 控制 ∥ 通信(MQTT)。先完成实验一 race-demo、实验二 snapshot-lock，再做本实验。

```bash
# 板端原生（推荐）
make clean && make
BROKER_HOST=<主机局域网IP> ./tri-thread

# 可选：主机交叉后再 scp
make CROSS_COMPILE=riscv64-ruyisdk-linux-gnu-
scp tri-thread user@board-ip:~/
```

Broker 用环境变量 `BROKER_HOST`（优先于源码默认值）。可选 `MQTT_CLIENT_ID`；默认 `tri-thread-<主机名>`。

默认 `USE_LOCK 0` 必现 `[RACE]`；验收改为 `USE_LOCK 1`（与实验二成对加锁同一思路）。
