# 实验 4.3：TCP Server 发送传感器 JSON

## 对应讲次

| 项目 | 内容 |
| --- | --- |
| 章节 | 第四章 文件系统与网络编程 |
| 讲次 | 第 3 讲 |
| 课程主题 | 网络编程基础 |
| 实验类型 | 必做实验 |

大纲讲次原文：第3讲 网络编程基础。
大纲实验原文：LicheePi 4A 作为 TCP Server，发送传感器 JSON 数据，PC 端编写 Client 接收并显示
大纲知识点原文：网络协议栈概述；TCP Socket 编程；UDP Socket 编程；字节序与数据封包；主机名与地址解析；Socket 选项与超时。

## 实验目标

- 在 LicheePi 4A 上编写 TCP Server。
- 使用“长度头 + JSON body”的简单协议发送传感器数据。
- 在 PC 端编写 Client 接收并显示 JSON。
- 验证断开重连、超时和端口占用等网络问题。

## 对应知识点

| # | 知识点 | 本实验中的验证方式 |
| --- | --- | --- |
| 1 | 网络协议栈概述 | 记录板端 IP、PC IP、端口和连通性 |
| 2 | TCP Socket 编程 | 使用 `socket/bind/listen/accept/send/recv` 完成服务端和客户端 |
| 3 | UDP Socket 编程 | 可选：用 UDP 发送一条广播发现消息 |
| 4 | 字节序与数据封包 | 4 字节网络字节序长度头 + UTF-8 JSON body |
| 5 | 主机名与地址解析 | PC Client 使用 `getaddrinfo` 解析板端地址 |
| 6 | Socket 选项与超时 | 使用 `SO_REUSEADDR`、收发超时和 keepalive |

## 实验环境

| 项目 | 要求 |
| --- | --- |
| 主机环境 | PC 与 LicheePi 4A 在同一局域网，能编译 Client |
| 目标板 | LicheePi 4A |
| 目标系统 | RevyOS |
| 硬件连接 | DHT22 或 mock 输入 |
| 软件依赖 | `gcc`、`make`、`iproute2`、`netcat-openbsd` 或 `nc`、`ss` |

## 硬件连接或部署关系

| 模块 | 引脚/接口 | 连接说明 | 注意事项 |
| --- | --- | --- | --- |
| TCP Server | 板端 `0.0.0.0:9000` | 监听 PC Client 连接 | 使用 `SO_REUSEADDR` 便于重复启动 |
| PC Client | PC 到 `<board-ip>:9000` | 接收长度头和 JSON | 防火墙需允许出站连接 |
| 传感器数据 | DHT22 或 mock | Server 每秒生成一条 JSON | 无硬件时使用 `--mock` |

## 实验任务

### 任务 1：定义 JSON 字段

每条数据包含 `device`、`timestamp_ms`、`temperature`、`humidity`、`valid`。

### 任务 2：实现 TCP Server

Server 监听 9000 端口，接受一个客户端连接后周期性发送封包。

### 任务 3：实现 PC Client

Client 读取 4 字节长度头，再循环读取完整 JSON body 并打印。

### 任务 4：验证异常场景

关闭客户端、重复启动服务端、拔掉网络或输入错误 IP，记录诊断信息。

## 实验步骤

```bash
# 板端准备
mkdir -p ch04_lab3/{include,src,build,bin}
cd ch04_lab3
gcc -Iinclude src/sensor_json.c src/tcp_server.c -o bin/sensor_tcp_server
hostname -I
```

```bash
# 板端启动 Server
./bin/sensor_tcp_server --host 0.0.0.0 --port 9000 --interval 1000 --mock
ss -lntp | grep 9000
```

```bash
# PC 端编译和运行 Client
gcc pc_client.c -o pc_client
./pc_client <board-ip> 9000
```

```bash
# 简单连通性检查
ping <board-ip>
nc -vz <board-ip> 9000
```

```bash
# 异常验证：关闭 Client 后重新连接
./pc_client <board-ip> 9000
# 按 Ctrl+C 结束，再次运行
./pc_client <board-ip> 9000
```

## 运行验证

| 验证项 | 预期现象 | 是否通过 |
| --- | --- | --- |
| 网络连通 | PC 能 ping 通板端 IP |  |
| 端口监听 | `ss -lntp` 显示 9000 正在监听 |  |
| Client 接收 | PC 每秒输出一条完整 JSON |  |
| 封包正确 | JSON 不截断、不粘连，长度头解析正确 |  |
| 断开重连 | Client 退出后 Server 不崩溃，可再次连接 |  |
| 超时诊断 | 网络异常时输出超时或断开日志 |  |

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
| `Connection refused` | Server 未启动或端口错误 | 检查 `ss -lntp` 和端口参数 |
| `Address already in use` | 旧进程未退出或端口处于占用状态 | `pgrep` 查找旧进程，确认设置 `SO_REUSEADDR` |
| Client 输出乱码 | 未按长度头读取完整 JSON | 检查 `ntohl` 和循环读取逻辑 |
| 连通性不稳定 | Wi-Fi 隔离或网络不同段 | 确认 PC 和板端在同一局域网 |

## 提交要求

- 实验记录：板端 IP、端口、JSON 字段、运行命令。
- 运行截图：Server 日志、PC Client 输出、`ss -lntp`。
- 源码或配置文件：TCP Server、PC Client、JSON 生成函数。
- 简短说明：说明为什么 TCP 需要处理消息边界。
