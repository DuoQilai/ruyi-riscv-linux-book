# mqtt-led · MQTT 远程控灯

荔枝派 4A：订阅命令主题驱动外接 LED，并向状态主题 publish。

依赖：libmosquitto、libgpiod。交叉前缀默认 `riscv64-unknown-linux-gnu-`（RuyiSDK）。

## 你要补全

1. 消息回调里根据 `on`/`off` 点灯  
2. `publish_status()` 向上行主题发布当前状态  

## 构建

```bash
make
scp mqtt-led user@board-ip:~/
ssh user@board-ip './mqtt-led'
```

把 `BROKER_HOST` 改成 Broker 的局域网 IP。主题默认 `course/led/cmd` 与 `course/led/status`。
