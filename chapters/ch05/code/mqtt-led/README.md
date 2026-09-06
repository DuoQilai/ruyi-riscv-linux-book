# mqtt-led · 第五章实验三（主实验）

荔枝派 4A：订阅命令主题驱动外接 LED，并向状态主题 publish。
先完成实验一 `led-cmd-parse`、实验二 `virt-led-repl`，再做本实验。

依赖：libmosquitto、libgpiod。板端原生 `make`；交叉：先激活 `venv-gnu-ruyisdk`，再 `make CROSS_COMPILE=riscv64-ruyisdk-linux-gnu-`。

## 你要补全

1. 消息回调里根据 `on`/`off` 点灯（`led_set` 成功后再发 status）  
2. `publish_status()` 向上行主题发布当前状态  

## Broker 与 Client ID

在跑 Mosquitto 的主机上先查局域网 IP：

```bash
hostname -I
```

板端：

```bash
BROKER_HOST=<主机局域网IP> ./mqtt-led
# 可选：MQTT_CLIENT_ID=班里唯一名字 ./mqtt-led
# 默认 client id 为 mqtt-led-<主机名>
```

主题默认 `course/led/cmd` 与 `course/led/status`。默认脚位：`LED_LINE=4`（丝印 `IO1_4`）。

## 构建

```bash
# 板端
make
BROKER_HOST=<主机局域网IP> ./mqtt-led

# 或主机交叉后 scp
make CROSS_COMPILE=riscv64-ruyisdk-linux-gnu-
scp mqtt-led user@board-ip:~/
```

参考实现（教师/自检）：`make sol` → `mqtt-led-sol`。
