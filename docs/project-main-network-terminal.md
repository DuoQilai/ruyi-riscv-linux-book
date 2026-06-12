# 第4章扩展参考：联网信息采集终端

## 项目定位

本文件保留旧版“联网信息采集终端”的项目定义，作为新版 6 章大纲中的第 4 章扩展参考，不再作为课程主线项目或独立章节。

新版课程主线以 `docs/course-outline.md` 和 `docs/course-plan.md` 为准：第 4 章章末综合项目是“手机 MQTT 远程灯光控制”。联网信息采集终端只能在第 4 章完成 MQTT 灯光项目之后，作为 HTTP 状态页、文件日志和多线程状态整合的选做练习使用，不能替代或压过 MQTT 控灯项目。

## 与新版课程的关系

| 新版位置 | 默认项目 | 本文件用途 |
| --- | --- | --- |
| ch04.3 网络编程基础 | TCP Server 发送传感器 JSON | 可复用其中的传感器数据结构和 Socket 验证思路 |
| ch04.4 并发服务器、HTTP 与 MQTT 服务 | 手机 MQTT 远程灯光控制 | MQTT 控灯是必做阶段项目；HTTP 状态页只能作为扩展展示 |
| ch06 边缘智能篇 | OpenCV、目标检测、TinyML、LLM/VLM | 不再把本项目延伸成 AI 主线 |

## 推荐使用边界

| 项目 | 新版默认方案 | 本文件可选扩展 | 说明 |
| --- | --- | --- | --- |
| 开发板 | LicheePi 4A | K1 / Muse Pi Pro 可作参考适配 | 课程主线统一为 LicheePi 4A + RevyOS |
| 操作系统 | RevyOS | 其他系统只作资料参考 | 镜像、登录、部署和验收说明应优先使用 RevyOS |
| 网络控制 | MQTT 控灯 | HTTP 状态页或 TCP 数据页 | MQTT topic、JSON 指令、PWM 调光和状态回传是第 4 章验收重点 |
| 数据来源 | 灯光状态、传感器 JSON | DHT22、ADC 或模拟数据 | 传感器数据可以服务于状态页展示 |
| 本地输出 | LED/PWM 灯光执行结果 | OLED、终端或日志摘要 | 不额外增加必做硬件负担 |

## 可复用能力

| 来源章节 | 复用能力 | 在扩展项目中的落点 |
| --- | --- | --- |
| ch01 | RevyOS 镜像、SSH/SCP、工具链、GDB、Shell 自动化 | 部署、运行、调试和服务管理 |
| ch02 | GPIO、按键、UART、PWM、ADC、I2C/SPI、DHT22、OLED | 灯光状态、传感器数据和本地显示 |
| ch03 | 静态/共享库、进程、IPC、pthread、生产者-消费者 | 采集、显示、日志、网络任务协同 |
| ch04 | 文件 I/O、JSON 配置、日志、Socket、HTTP、MQTT/mosquitto | 扩展 HTTP 状态页，辅助展示 MQTT 控灯状态 |

## 功能模块

| 模块 | 职责 | 输入 | 输出 |
| --- | --- | --- | --- |
| 配置模块 | 读取采样周期、HTTP 端口、MQTT topic 和日志级别 | `config/app.json` | 运行参数 |
| MQTT 模块 | 接收手机 MQTT 客户端指令并发布状态 | topic、JSON payload | 灯光动作和状态回传 |
| 灯光模块 | GPIO/PWM 执行开关和亮度调节 | MQTT 指令 | LED 或灯光模块状态 |
| 采集模块 | 可选读取 DHT22、ADC 或模拟数据 | 传感器或模拟数据源 | 环境数据结构 |
| 状态模块 | 维护灯光、传感器、错误和网络状态 | 各模块事件 | 共享状态 |
| 日志模块 | 写入运行日志、数据日志和错误日志 | 状态变化、指令、错误事件 | `logs/`、`data/` |
| HTTP 模块 | 可选发布状态页 | 共享状态、日志摘要 | 浏览器可访问页面 |

## 线程结构建议

```text
main
├── config load
├── mqtt_thread         # 必做：订阅控制 topic，发布状态 topic
├── light_thread        # 必做：执行 GPIO/PWM 灯光动作
├── sensor_thread       # 可选：采集 DHT22/ADC/模拟数据
├── log_thread          # 可选：写入数据日志和运行日志
└── http_thread         # 可选：发布 HTTP 状态页
```

线程间共享一份 `system_state`，至少包含灯光开关、亮度、最近 MQTT 指令、网络状态、最近错误和更新时间。共享状态必须通过互斥锁、条件变量或消息队列保护。

## 目录结构建议

```text
ch04_mqtt_light_optional_status/code/
├── Makefile
├── README.md
├── config/
│   └── app.json
├── data/
│   └── samples.csv
├── logs/
│   ├── app.log
│   └── error.log
└── src/
    ├── main.c
    ├── config.c
    ├── mqtt_client.c
    ├── light.c
    ├── sensor.c
    ├── logger.c
    └── http_status.c
```

## HTTP 状态页扩展

HTTP 状态页是可选展示能力，用来查看 MQTT 控灯项目的运行状态。它不承担第 4 章默认验收的网络控制入口。

状态页可显示：

- 项目名称：MQTT 远程灯光控制状态页
- 灯光状态：开关、亮度、模式
- 最近 MQTT topic 和 payload 摘要
- 状态回传时间
- 可选温度、湿度、ADC 或模拟数据
- 程序运行时长
- 最近错误摘要
- 日志文件位置或最近日志摘要

验收 URL 建议：

```text
http://<licheepi4a-ip>:8080/
```

## 实验拆分

| 实验 | 任务 | 验收 |
| --- | --- | --- |
| Lab 1 | 完成第 4 章默认 MQTT 控灯项目 | 手机 MQTT 客户端可开关灯并调节亮度，板端回传状态 |
| Lab 2 | 增加共享状态和日志记录 | MQTT 指令、灯光状态和错误日志可追踪 |
| Lab 3 | 可选增加 HTTP 状态页 | 浏览器能看到最新灯光状态和最近更新时间 |

## 验收标准

- 必须先满足第 4 章默认项目：手机 MQTT 客户端能远程开关灯、调节亮度并看到状态回传。
- 程序能在 LicheePi 4A + RevyOS 或等效实验环境中构建和运行。
- `logs/` 或终端输出中能看到可读的 MQTT 指令、灯光状态和错误记录。
- 若实现 HTTP 状态页，浏览器访问页面能看到最新灯光状态、最近指令和更新时间。
- 真实传感器不可用时，允许使用模拟数据，但不得影响 MQTT 控灯主流程验收。

## 不做内容

- 不把联网信息采集终端恢复为独立章节或课程主线作品。
- 不用 HTTP 状态页替代第 4 章 MQTT 控灯项目。
- 不强制所有学生实现复杂 Web UI、云平台或多板适配。
- 不在本项目重新讲授 GPIO、PWM、文件 I/O、pthread、Socket 或 MQTT 基础。
- 不改变课程主线板卡：第一轮课程材料以 LicheePi 4A + RevyOS 为准，K1 / Muse Pi Pro 只作扩展或参考。

## 风险与 fallback

| 风险 | 影响 | fallback |
| --- | --- | --- |
| MQTT broker 配置失败 | 手机无法控制灯光 | 先在板端或 PC 端用 `mosquitto_pub/sub` 验证 topic，再接手机客户端 |
| PWM 调光不稳定 | 亮度调节不可验收 | 降级为 GPIO 开关灯，保留 payload 和状态回传 |
| 传感器不可用 | 状态页数据不完整 | 使用模拟数据或只展示灯光状态 |
| HTTP 页面无法访问 | 扩展展示不可见 | 保留 MQTT 状态回传和日志作为验收依据 |
| 多线程竞争 | 状态显示和日志异常 | 降级为单线程事件循环或 MQTT + 灯光两线程 |
