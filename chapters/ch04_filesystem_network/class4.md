# 4.4 并发服务器、HTTP 与 MQTT 服务

## 对应大纲

大纲讲次原文：第4讲 并发服务器、HTTP 与 MQTT 服务。
大纲知识点原文：多进程并发服务器；多线程并发服务器；I/O 多路复用；HTTP 协议基础；嵌入式 HTTP 服务器实现；MQTT 与 mosquitto 消息通信。

多进程并发服务器、多线程并发服务器、I/O 多路复用、HTTP 协议、嵌入式 HTTP 服务器实现、MQTT 与 mosquitto 消息通信。

## 目标

学生能理解常见并发服务器模型，能用 HTTP 做状态展示，并能基于 mosquitto 与手机 MQTT 客户端完成远程灯光开关、PWM 亮度调节和状态回传。

## 知识点

| # | 知识点 | 内容 |
| --- | --- | --- |
| 1 | 多进程并发服务器 | fork-per-connection、`SIGCHLD`、prefork 和资源成本 |
| 2 | 多线程并发服务器 | thread-per-connection、线程池、连接数上限与拒绝服务防护 |
| 3 | I/O 多路复用 | `select`、`poll`、`epoll`、边缘触发与水平触发 |
| 4 | HTTP 协议基础 | 请求/响应、GET/POST、状态码、Content-Type、URL 解码 |
| 5 | 嵌入式 HTTP 服务器实现 | epoll 单线程事件驱动、路由、静态文件和状态页 |
| 6 | MQTT 与 mosquitto 消息通信 | broker/client、topic、publish/subscribe、JSON 指令、状态回传、手机接入 |

## 讲授要点

- 多进程模型隔离性好但开销较高，多线程模型共享数据方便但要处理同步，epoll 适合大量连接的事件驱动服务。
- HTTP 适合浏览器查看状态和简单配置，MQTT 适合设备控制、状态上报和手机客户端交互。
- MQTT 以 broker 为中心，板端和手机都连接 broker；手机发布控制 topic，板端订阅控制 topic 并发布状态 topic。
- 本项目建议 topic 使用 `rv/lpi4a/light/cmd` 和 `rv/lpi4a/light/status`，避免使用过宽的通配符订阅。
- 控制 payload 使用 JSON，例如 `{"power":true,"brightness":60}`；板端必须校验字段类型和取值范围。
- PWM 占空比建议按 0 到 100 的亮度百分比映射到 `duty_cycle = period * brightness / 100`，关灯时 duty 设为 0。
- 断线和权限问题是项目验收重点：broker 未启动、账号密码错误、topic 不一致、GPIO/PWM 权限不足都要能诊断。

## 操作或演示

1. 安装并启动 mosquitto。

```bash
sudo apt update
sudo apt install -y mosquitto mosquitto-clients
sudo systemctl enable --now mosquitto
systemctl status mosquitto --no-pager
```

2. 在板端或 PC 上做 MQTT 自测。

```bash
mosquitto_sub -h <broker-ip> -t 'rv/lpi4a/light/#' -v
mosquitto_pub -h <broker-ip> -t 'rv/lpi4a/light/cmd' -m '{"power":true,"brightness":60}'
```

3. 配置板端灯光服务。

```json
{
  "mqtt": {
    "host": "127.0.0.1",
    "port": 1883,
    "client_id": "lpi4a-light-01",
    "cmd_topic": "rv/lpi4a/light/cmd",
    "status_topic": "rv/lpi4a/light/status"
  },
  "light": {
    "gpio_chip": "gpiochip0",
    "gpio_line": 0,
    "pwm_chip": 0,
    "pwm_channel": 0,
    "period_ns": 1000000
  }
}
```

4. 运行板端服务并订阅状态。

```bash
./build/light_mqtt_service --config config/light.json
mosquitto_sub -h <broker-ip> -t 'rv/lpi4a/light/status' -v
```

5. 手机 MQTT 客户端配置。

```text
Broker: <broker-ip>
Port: 1883
Publish topic: rv/lpi4a/light/cmd
Subscribe topic: rv/lpi4a/light/status
Payload: {"power":true,"brightness":80}
```

## 运行验证

| 验证项 | 命令或操作 | 预期现象 |
| --- | --- | --- |
| broker 运行 | `systemctl status mosquitto` | mosquitto 为 active |
| topic 自测 | `mosquitto_pub/sub` | 订阅端收到控制 JSON |
| 开灯 | 手机发布 `{"power":true,"brightness":70}` | 灯亮，PWM 占空比约 70% |
| 调光 | 手机发布 `{"power":true,"brightness":20}` | 灯明显变暗，状态回传 brightness 为 20 |
| 关灯 | 手机发布 `{"power":false,"brightness":0}` | 灯灭，状态 topic 回传 power false |
| 错误 payload | 发布 `{"brightness":150}` | 板端拒绝并回传 error 状态或记录 WARN |
| 断线恢复 | 停止再启动 broker | 板端记录断线并重连，恢复后继续接收指令 |

## 常见问题

| 现象 | 可能原因 | 处理方式 |
| --- | --- | --- |
| 手机连接不上 broker | 手机和板端不在同一局域网，端口被防火墙阻挡 | 确认 IP、Wi-Fi、1883 端口和 broker 监听地址 |
| 板端收不到指令 | topic 拼写不一致或订阅了错误层级 | 用 `mosquitto_sub -v -t 'rv/lpi4a/light/#'` 抓取所有相关消息 |
| 灯不亮但 MQTT 正常 | GPIO/PWM 编号、权限或接线错误 | 回到第二章 GPIO/PWM 示例单独验证硬件 |
| 亮度无变化 | PWM period/duty 配置错误或灯光模块不支持 PWM | 检查 `/sys/class/pwm`，用示波器或肉眼低频测试 |
| 状态不回传 | 发布状态 topic 失败或 JSON 构造错误 | 检查服务日志和 `mosquitto_sub` 输出 |

## 本讲成果

- 能比较多进程、多线程和 epoll 并发模型。
- 能用 HTTP 作为可选状态展示入口。
- 能配置 mosquitto broker、topic 和手机 MQTT 客户端。
- 能实现 MQTT JSON 指令到 GPIO/PWM 控制的闭环，并发布状态回传。
