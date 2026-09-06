# mqtt-led · 第五章实验三（主实验）

荔枝派 4A：订阅命令主题驱动外接 LED，并向状态主题 publish。
先做完实验一、实验二；本实验 Makefile 已链上实验一的 `parse_led_cmd` / `status_payload`。

依赖：libmosquitto、libgpiod。**推荐板端原生 `make`**（库现成）。主机交叉：先 `source ~/venv-gnu-ruyisdk/bin/ruyi-activate`，再 `make CROSS_COMPILE=riscv64-ruyisdk-linux-gnu-`（需交叉环境里能链到同名库）。

## 你要补全

1. `on_message`：用 `parse_led_cmd` → `led_set` 成功后再 `publish_status(status_payload(...))`  
2. `publish_status()`：`mosquitto_publish` 发到状态主题  

## Broker 与 Client ID

```bash
hostname -I   # 在跑 Mosquitto 的主机上
BROKER_HOST=<主机局域网IP> ./mqtt-led
# 可选 MQTT_CLIENT_ID=… ；默认 mqtt-led-<主机名>
```

主题默认 `course/led/cmd` / `course/led/status`。脚位默认 `LED_LINE=4`（`IO1_4`）。

## 构建

```bash
make
BROKER_HOST=<主机局域网IP> ./mqtt-led
# 教师/自检：make sol → mqtt-led-sol
```
