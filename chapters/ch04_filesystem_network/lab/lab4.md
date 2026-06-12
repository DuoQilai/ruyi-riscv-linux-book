# 综合项目 4.4：手机 MQTT 远程灯光控制

## 对应讲次

| 项目 | 内容 |
| --- | --- |
| 章节 | 第四章 文件系统与网络编程 |
| 讲次 | 第 4 讲 |
| 课程主题 | 并发服务器、HTTP 与 MQTT 服务 |
| 实验类型 | 阶段综合项目 |

大纲讲次原文：第4讲 并发服务器、HTTP 与 MQTT 服务。
大纲实验原文：LicheePi 4A 连接灯光模块，基于 mosquitto 接收手机 MQTT 程序发出的指令，结合 GPIO/PWM 实现远程开关与亮度调节
大纲知识点原文：多进程并发服务器；多线程并发服务器；I/O 多路复用；HTTP 协议基础；嵌入式 HTTP 服务器实现；MQTT 与 mosquitto 消息通信。

## 实验目标

- 在 LicheePi 4A 或局域网主机上运行 mosquitto broker。
- 手机 MQTT 客户端发布 JSON 控制指令。
- 板端订阅控制 topic，解析 `power` 和 `brightness` 字段。
- 板端使用 GPIO/PWM 控制灯光模块开关和亮度。
- 板端向状态 topic 发布当前状态、错误码和在线信息。
- 验证断线、权限、topic 错误和 payload 错误的调试流程。

## 对应知识点

| # | 知识点 | 本实验中的验证方式 |
| --- | --- | --- |
| 1 | 多进程并发服务器 | 了解 broker 作为独立服务与板端控制进程的关系 |
| 2 | 多线程并发服务器 | 板端服务可用 MQTT 线程和硬件控制线程分工 |
| 3 | I/O 多路复用 | 观察 MQTT 客户端库内部事件循环或使用 `poll` 驱动 |
| 4 | HTTP 协议基础 | 可选：提供只读状态页查看灯光状态 |
| 5 | 嵌入式 HTTP 服务器实现 | 可选：在 `/status` 返回当前 JSON 状态 |
| 6 | MQTT 与 mosquitto 消息通信 | 配置 broker、topic、JSON 指令、状态回传和手机接入 |

## 实验环境

| 项目 | 要求 |
| --- | --- |
| 主机环境 | PC、手机、LicheePi 4A 在同一局域网 |
| 目标板 | LicheePi 4A |
| 目标系统 | RevyOS |
| 硬件连接 | 灯光模块接 GPIO/PWM；模块供电与板端共地 |
| 软件依赖 | `mosquitto`、`mosquitto-clients`、`libmosquitto-dev`、`json-c` 或 `cJSON`、GPIO/PWM 控制库 |

## 硬件连接或部署关系

| 模块 | 引脚/接口 | 连接说明 | 注意事项 |
| --- | --- | --- | --- |
| mosquitto broker | `<broker-ip>:1883` | 可运行在 LicheePi 4A，也可运行在 PC | 手机和板端必须能访问该 IP 和端口 |
| 手机 MQTT 客户端 | Wi-Fi | 发布控制 topic，订阅状态 topic | 关闭移动网络干扰，确认在同一局域网 |
| 灯光开关 | 课程指定 GPIO | 控制使能脚或继电器输入 | 继电器/灯带供电电流不得由 GPIO 直接承担 |
| 亮度调节 | 课程指定 PWM | PWM 输出接 MOSFET/驱动模块控制端 | PWM 编号以 LicheePi 4A 实测为准 |
| 共地 | GND | 灯光模块电源地与开发板 GND 相连 | 未共地会导致控制信号不稳定 |

## 实验任务

### 任务 1：启动 broker 并完成 topic 自测

安装 mosquitto，确认 `rv/lpi4a/light/cmd` 和 `rv/lpi4a/light/status` 能正常收发。

### 任务 2：实现板端 MQTT 控灯服务

服务订阅控制 topic，解析 JSON 指令，调用 GPIO/PWM 控制灯光，并记录日志。

### 任务 3：配置手机 MQTT 客户端

手机连接 broker，发布开灯、关灯、调光指令，订阅状态 topic 查看回传。

### 任务 4：完成异常与安全验证

验证 broker 断开、账号密码错误、topic 拼写错误、payload 非法、GPIO/PWM 权限不足等情况。

## 实验步骤

```bash
# 板端安装并启动 broker
sudo apt update
sudo apt install -y mosquitto mosquitto-clients libmosquitto-dev
sudo systemctl enable --now mosquitto
systemctl status mosquitto --no-pager
hostname -I
```

```bash
# 如果 broker 只允许本机访问，创建课程实验配置
sudo tee /etc/mosquitto/conf.d/rv-course.conf >/dev/null <<'EOF'
listener 1883 0.0.0.0
allow_anonymous true
EOF
sudo systemctl restart mosquitto
ss -lntp | grep 1883
```

```bash
# topic 自测：终端 1 订阅
mosquitto_sub -h <broker-ip> -t 'rv/lpi4a/light/#' -v
```

```bash
# topic 自测：终端 2 发布
mosquitto_pub -h <broker-ip> -t 'rv/lpi4a/light/cmd' -m '{"power":true,"brightness":60}'
```

```bash
# PWM 手动验证，编号按实测修改
PWMCHIP=/sys/class/pwm/pwmchip0
echo 0 | sudo tee $PWMCHIP/export
echo 1000000 | sudo tee $PWMCHIP/pwm0/period
echo 500000 | sudo tee $PWMCHIP/pwm0/duty_cycle
echo 1 | sudo tee $PWMCHIP/pwm0/enable
```

```bash
# 板端服务配置
mkdir -p ch04_lab4/{config,src,include,build,bin,log}
cat > ch04_lab4/config/light.json <<'EOF'
{
  "mqtt": {
    "host": "127.0.0.1",
    "port": 1883,
    "client_id": "lpi4a-light-01",
    "cmd_topic": "rv/lpi4a/light/cmd",
    "status_topic": "rv/lpi4a/light/status",
    "keepalive": 30
  },
  "light": {
    "gpio_chip": "gpiochip0",
    "gpio_line": 0,
    "pwm_chip": 0,
    "pwm_channel": 0,
    "period_ns": 1000000
  },
  "log": {
    "level": "INFO",
    "file": "log/light.log"
  }
}
EOF
```

```bash
# 编译和运行板端服务
gcc -Iinclude src/light_gpio_pwm.c src/light_mqtt_service.c \
  -lmosquitto $(pkg-config --cflags --libs json-c) -o bin/light_mqtt_service
./bin/light_mqtt_service --config config/light.json
```

手机 MQTT 客户端配置：

| 项目 | 配置值 |
| --- | --- |
| Broker | `<broker-ip>` |
| Port | `1883` |
| Client ID | `phone-light-test` |
| Subscribe topic | `rv/lpi4a/light/status` |
| Publish topic | `rv/lpi4a/light/cmd` |
| QoS | `0` 或 `1`，课堂默认 `0` |

控制 payload 示例：

```json
{"power":true,"brightness":100}
```

```json
{"power":true,"brightness":30}
```

```json
{"power":false,"brightness":0}
```

状态 topic 回传示例：

```json
{
  "device": "lpi4a-light-01",
  "power": true,
  "brightness": 60,
  "pwm": {
    "period_ns": 1000000,
    "duty_cycle_ns": 600000
  },
  "online": true,
  "error": null
}
```

PWM 占空比计算规则：

```text
duty_cycle_ns = period_ns * brightness / 100
brightness = 0 时 duty_cycle_ns = 0
power = false 时关闭 GPIO 使能并将 duty_cycle_ns 设为 0
brightness 超出 0..100 时拒绝执行并发布错误状态
```

断线与权限调试：

```bash
# broker 断线
sudo systemctl stop mosquitto
tail -f ch04_lab4/log/light.log
sudo systemctl start mosquitto

# topic 抓包式观察
mosquitto_sub -h <broker-ip> -t 'rv/lpi4a/light/#' -v

# 权限检查
ls -l /sys/class/pwm/
groups
sudo ./bin/light_mqtt_service --config config/light.json
```

## 运行验证

| 验证项 | 预期现象 | 是否通过 |
| --- | --- | --- |
| broker 启动 | `systemctl status mosquitto` 显示 active |  |
| topic 自测 | `mosquitto_sub` 能收到 `cmd` 测试消息 |  |
| 手机连接 | 手机 App 显示 connected |  |
| 开灯 | 发布 `{"power":true,"brightness":100}` 后灯亮 |  |
| 调光 | 发布 `brightness` 为 30、60、90 后亮度变化明显 |  |
| 关灯 | 发布 `{"power":false,"brightness":0}` 后灯灭 |  |
| 状态回传 | `rv/lpi4a/light/status` 返回 power、brightness、duty_cycle |  |
| 非法 payload | `brightness` 为 150 时服务拒绝并回传 error |  |
| 断线恢复 | broker 重启后服务能重连或输出明确重连日志 |  |
| 权限诊断 | GPIO/PWM 无权限时日志指出具体设备路径 |  |

## 验收记录

| 记录项 | 内容 |
| --- | --- |
| broker IP 和端口 |  |
| 控制 topic | `rv/lpi4a/light/cmd` |
| 状态 topic | `rv/lpi4a/light/status` |
| 开灯 payload |  |
| 调光 payload |  |
| 关灯 payload |  |
| PWM period/duty 实测值 |  |
| 手机截图或录屏 |  |
| 灯光模块照片 |  |
| 异常处理记录 |  |

## 常见问题

| 现象 | 可能原因 | 处理方式 |
| --- | --- | --- |
| 手机无法连接 broker | IP 写错、不同局域网、mosquitto 只监听本机 | 检查 `<broker-ip>`、Wi-Fi、`ss -lntp` 和 mosquitto 配置 |
| 订阅端收不到消息 | topic 不一致或通配符写错 | 使用 `rv/lpi4a/light/#` 临时观察全量消息 |
| 服务收到消息但灯不亮 | GPIO/PWM 编号或接线错误 | 先用手动 GPIO/PWM 命令单独验证 |
| 调光没有变化 | 灯光模块不支持 PWM 或 PWM 未 enable | 检查 `period`、`duty_cycle`、`enable` 和驱动模块说明 |
| 状态 topic 没有回传 | 板端发布失败或 JSON 构造失败 | 查看服务日志，使用 `mosquitto_sub -v` 订阅状态 topic |
| broker 重启后服务不恢复 | MQTT 客户端未设置重连 | 设置重连策略并记录断线、重连日志 |
| 提示权限不足 | 普通用户无 GPIO/PWM 访问权限 | 调整 udev/用户组，课堂验证可临时使用 `sudo` |

## 提交要求

- 实验记录：broker 地址、topic、payload、PWM period 和 duty_cycle 实测值。
- 运行截图：mosquitto 状态、手机连接、状态 topic 回传、服务日志。
- 源码或配置文件：`light.json`、MQTT 服务源码、GPIO/PWM 控制代码。
- 简短说明：说明 JSON 指令如何映射为 GPIO/PWM 动作，以及断线和权限问题如何排查。
