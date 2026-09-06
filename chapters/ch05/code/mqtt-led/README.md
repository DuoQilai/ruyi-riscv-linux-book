# mqtt-led · MQTT 远程控灯

荔枝派 4A：订阅命令主题驱动外接 LED，并向状态主题 publish。

依赖：libmosquitto、libgpiod。交叉：先激活 `venv-gnu-ruyisdk`，再 `make CROSS_COMPILE=riscv64-ruyisdk-linux-gnu-`。

## 你要补全

1. 消息回调里根据 `on`/`off` 点灯  
2. `publish_status()` 向上行主题发布当前状态  

## Broker 地址（不要硬编码别人的 IP）

在跑 Mosquitto 的主机上先查局域网 IP：

```bash
hostname -I
```

板端任选其一：

```bash
# 推荐：运行时指定（优先于源码默认值）
BROKER_HOST=<主机局域网IP> ./mqtt-led

# 或改 main.c 里的 DEFAULT_BROKER_HOST 后重新 make
```

主题默认 `course/led/cmd` 与 `course/led/status`。默认脚位：`LED_LINE=4`（丝印 `IO1_4`）。

## 构建

```bash
make
scp mqtt-led user@board-ip:~/
ssh user@board-ip 'BROKER_HOST=<主机局域网IP> ./mqtt-led'
```

参考实现（教师/自检）：`make sol` → `mqtt-led-sol`。
