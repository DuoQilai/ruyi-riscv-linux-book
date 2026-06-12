# 实验 4.2：JSON 配置与多级日志

## 对应讲次

| 项目 | 内容 |
| --- | --- |
| 章节 | 第四章 文件系统与网络编程 |
| 讲次 | 第 2 讲 |
| 课程主题 | 配置管理与日志系统 |
| 实验类型 | 必做实验 |

大纲讲次原文：第2讲 配置管理与日志系统。
大纲实验原文：为传感器终端添加 JSON 配置（采样频率/阈值/OLED 开关）+ 多级日志系统
大纲知识点原文：命令行参数解析；INI 配置文件解析；JSON 配置文件解析；日志级别设计；日志输出与轮转；工程目录结构规范。

## 实验目标

- 为传感器终端添加 JSON 配置文件。
- 使用命令行参数覆盖配置项。
- 实现 DEBUG、INFO、WARN、ERROR 多级日志。
- 实现配置校验、日志文件输出和简单日志轮转。

## 对应知识点

| # | 知识点 | 本实验中的验证方式 |
| --- | --- | --- |
| 1 | 命令行参数解析 | 使用 `--config`、`--interval`、`--log-level` 覆盖默认配置 |
| 2 | INI 配置文件解析 | 课堂对比 INI 结构，本实验重点采用 JSON |
| 3 | JSON 配置文件解析 | 使用 `json-c` 或 `cJSON` 解析采样、阈值、显示和日志配置 |
| 4 | 日志级别设计 | 不同级别输出不同详细程度的日志 |
| 5 | 日志输出与轮转 | 同时输出到终端和文件，达到大小阈值后轮转 |
| 6 | 工程目录结构规范 | 整理 `src/include/config/log/build/bin` 目录 |

## 实验环境

| 项目 | 要求 |
| --- | --- |
| 主机环境 | 能编译并部署 C 程序 |
| 目标板 | LicheePi 4A |
| 目标系统 | RevyOS |
| 硬件连接 | DHT22 或 mock 输入；可选 OLED |
| 软件依赖 | `gcc`、`make`、`pkg-config`、`libjson-c-dev` 或课程提供的 `cJSON` |

## 硬件连接或部署关系

| 模块 | 引脚/接口 | 连接说明 | 注意事项 |
| --- | --- | --- | --- |
| 配置文件 | `config/sensor.json` | 保存采样间隔、阈值、显示和日志配置 | JSON 语法错误应被程序提示 |
| 日志文件 | `log/sensor.log` | 保存运行日志 | 目录不存在时程序应创建或报错 |
| 采集模块 | DHT22 GPIO 或 mock | 按配置间隔采样 | 配置中的 GPIO 编号以实测为准 |

## 实验任务

### 任务 1：编写 JSON 配置

配置包含 `sample_interval_ms`、`temperature_warn`、`display.console`、`log.level`、`log.file` 和 `log.max_size_kb`。

### 任务 2：实现配置加载和校验

程序启动时读取 JSON，检查数值范围和字段类型，缺省字段使用默认值。

### 任务 3：实现命令行覆盖

命令行参数优先级高于配置文件，例如 `--interval 500` 覆盖 JSON 中的采样间隔。

### 任务 4：实现日志和轮转

日志同时输出到终端和文件；当日志文件超过阈值时重命名旧文件并创建新文件。

## 实验步骤

```bash
mkdir -p ch04_lab2/{include,src,build,bin,config,log}
cd ch04_lab2
cat > config/sensor.json <<'EOF'
{
  "sample_interval_ms": 1000,
  "temperature_warn": 35.0,
  "display": {
    "console": true,
    "oled": false
  },
  "log": {
    "level": "INFO",
    "file": "log/sensor.log",
    "max_size_kb": 256
  }
}
EOF
```

```bash
# 使用 json-c 时的编译示例
gcc -Iinclude src/config.c src/logger.c src/sensor_app.c \
  $(pkg-config --cflags --libs json-c) -o bin/sensor_app
```

```bash
# 使用配置文件运行
./bin/sensor_app --config config/sensor.json --mock
tail -n 20 log/sensor.log
```

```bash
# 命令行覆盖配置
./bin/sensor_app --config config/sensor.json --interval 500 --log-level DEBUG --mock
```

```bash
# 配置错误验证
cp config/sensor.json config/bad.json
printf '{ "sample_interval_ms": 0, "log": { "level": 123 } }\n' > config/bad.json
./bin/sensor_app --config config/bad.json --mock
```

```bash
# 降低轮转阈值以触发轮转
./bin/sensor_app --config config/sensor.json --log-max-size 16 --log-level DEBUG --mock
ls -lh log/
```

## 运行验证

| 验证项 | 预期现象 | 是否通过 |
| --- | --- | --- |
| 配置加载 | 程序启动日志显示配置路径和关键参数 |  |
| 参数覆盖 | `--interval 500` 后采样频率提高 |  |
| 日志级别 | DEBUG 模式输出更多调试信息 |  |
| 配置校验 | 非法 JSON 或非法字段输出明确错误 |  |
| 日志文件 | `log/sensor.log` 持续追加运行记录 |  |
| 日志轮转 | 达到阈值后生成旧日志文件 |  |

## 验收记录

| 记录项 | 内容 |
| --- | --- |
| 运行命令 |  |
| 关键输出 |  |
| 截图或照片 |  |
| 异常处理 |  |

## 常见问题

| 现象 | 可能原因 | 处理方式 |
| --- | --- | --- |
| 编译找不到 json-c | 未安装开发包或 pkg-config 不可用 | 安装 `libjson-c-dev`，或改用课程提供的 cJSON |
| 修改配置不生效 | 程序只在启动时读取配置 | 重启程序，或实现 `SIGHUP` 热加载 |
| 日志文件无法打开 | `log/` 目录不存在或权限不足 | 创建目录并确认运行用户可写 |
| 日志轮转丢失内容 | 轮转时未刷新文件缓冲 | 轮转前 `fflush` 并关闭旧文件 |

## 提交要求

- 实验记录：配置文件内容、命令行覆盖命令、错误配置案例。
- 运行截图：启动日志、DEBUG 日志、轮转后的 `ls -lh log/`。
- 源码或配置文件：`config.c`、`logger.c`、`sensor_app.c`、`sensor.json`。
- 简短说明：说明配置文件、命令行参数和日志级别的优先级。
