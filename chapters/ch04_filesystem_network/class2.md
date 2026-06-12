# 4.2 配置管理与日志系统

## 对应大纲

大纲讲次原文：第2讲 配置管理与日志系统。
大纲知识点原文：命令行参数解析；INI 配置文件解析；JSON 配置文件解析；日志级别设计；日志输出与轮转；工程目录结构规范。

命令行参数解析、INI 配置文件解析、JSON 配置文件解析、日志级别设计、日志输出与轮转、工程目录结构规范。

## 目标

学生能为传感器终端添加命令行参数、JSON 配置和多级日志，使采样频率、阈值、显示开关和日志级别可配置，并能通过日志定位运行问题。

## 知识点

| # | 知识点 | 内容 |
| --- | --- | --- |
| 1 | 命令行参数解析 | `getopt`、`getopt_long`、参数验证和 `--help` |
| 2 | INI 配置文件解析 | section/key 结构、默认值、简单配置场景 |
| 3 | JSON 配置文件解析 | `cJSON` 或 `json-c`、类型检查、schema 思路和热加载 |
| 4 | 日志级别设计 | DEBUG、INFO、WARN、ERROR、FATAL 与编译期/运行期级别 |
| 5 | 日志输出与轮转 | stdout、文件、syslog、按大小或时间轮转 |
| 6 | 工程目录结构规范 | `src/include/build/doc/log/config` 布局和模块边界 |

## 讲授要点

- 命令行参数适合临时覆盖，配置文件适合长期运行参数；建议优先级为命令行高于配置文件，高于默认值。
- 配置解析必须校验范围，例如采样间隔不能为 0，PWM 占空比必须在 0 到 100 之间。
- JSON 适合嵌套结构和网络 payload，INI 更适合简单键值；本课程后续 MQTT payload 使用 JSON，因此本讲重点使用 JSON。
- 日志应包含时间、级别、模块和消息，错误日志要能指导下一步排查，而不是只输出 “failed”。
- 嵌入式设备存储有限，长期写文件日志需要轮转或限制大小。
- 目录结构是工程沟通方式：源码、头文件、配置、日志、构建产物和文档应明确分离。

## 操作或演示

1. 定义配置文件。

```json
{
  "sample_interval_ms": 1000,
  "temperature_warn": 35.0,
  "display": {
    "oled": true,
    "console": true
  },
  "log": {
    "level": "INFO",
    "file": "log/sensor.log",
    "max_size_kb": 256
  }
}
```

2. 编译配置和日志示例。

```bash
pkg-config --cflags --libs json-c
gcc -Iinclude src/config.c src/logger.c app/sensor_app.c $(pkg-config --cflags --libs json-c) -o build/sensor_app
```

3. 使用命令行覆盖配置。

```bash
./build/sensor_app --config config/sensor.json --interval 500 --log-level DEBUG
```

4. 修改配置并触发热加载。

```bash
sed -i 's/"INFO"/"DEBUG"/' config/sensor.json
kill -HUP $(pidof sensor_app)
tail -n 20 log/sensor.log
```

5. 验证日志轮转。

```bash
./build/sensor_app --config config/sensor.json --log-max-size 32
ls -lh log/
```

## 运行验证

| 验证项 | 命令 | 预期现象 |
| --- | --- | --- |
| help 输出 | `./build/sensor_app --help` | 显示参数说明和默认值 |
| 配置加载 | `./build/sensor_app --config config/sensor.json` | 日志显示配置项和运行参数 |
| 参数覆盖 | `--interval 500` | 采样间隔变为 500 ms |
| 配置错误 | 写入非法 JSON | 程序提示具体字段或解析错误 |
| 日志级别 | `--log-level DEBUG` | DEBUG 日志出现；INFO 模式下不出现 |
| 日志轮转 | 长时间运行或降低大小阈值 | 生成 `.1` 或带时间戳的旧日志 |

## 常见问题

| 现象 | 可能原因 | 处理方式 |
| --- | --- | --- |
| 找不到 json-c | 开发包未安装或 pkg-config 路径缺失 | 安装 `libjson-c-dev`，或使用课程提供的 cJSON 源码 |
| 配置改了不生效 | 程序未实现热加载或未收到信号 | 重启程序，或发送 `SIGHUP` |
| 日志文件无法创建 | `log/` 目录不存在或权限不足 | 启动前创建目录并检查运行用户 |
| 日志刷屏 | 默认级别过低或循环错误 | 调高日志级别，采样循环添加合理间隔 |

## 本讲成果

- 能实现命令行参数和 JSON 配置加载。
- 能设计配置默认值、范围校验和错误提示。
- 能实现多级日志输出和简单轮转。
- 能整理出适合后续网络项目复用的工程目录结构。
