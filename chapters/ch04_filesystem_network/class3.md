# 4.3 网络编程基础

## 对应大纲

大纲讲次原文：第3讲 网络编程基础。
大纲知识点原文：网络协议栈概述；TCP Socket 编程；UDP Socket 编程；字节序与数据封包；主机名与地址解析；Socket 选项与超时。

网络协议栈概述、TCP Socket 编程、UDP Socket 编程、字节序与数据封包、主机名与地址解析、Socket 选项与超时。

## 目标

学生能在 LicheePi 4A + RevyOS 上编写 TCP Server，向 PC 客户端发送传感器 JSON 数据，并理解 TCP/UDP、端口、字节序、地址解析和超时处理。

## 知识点

| # | 知识点 | 内容 |
| --- | --- | --- |
| 1 | 网络协议栈概述 | OSI/TCP-IP、IP、子网、TCP 与 UDP、端口 |
| 2 | TCP Socket 编程 | `socket`、`bind`、`listen`、`accept`、`connect`、粘包处理 |
| 3 | UDP Socket 编程 | `sendto`、`recvfrom`、广播、组播和丢包应对 |
| 4 | 字节序与数据封包 | `htonl`、`htons`、结构体对齐、定长头 + JSON body |
| 5 | 主机名与地址解析 | `getaddrinfo`、`getnameinfo`、IPv4/IPv6 双栈 |
| 6 | Socket 选项与超时 | `SO_REUSEADDR`、`SO_KEEPALIVE`、`TCP_NODELAY`、收发超时 |

## 讲授要点

- TCP 是可靠字节流，不保留消息边界；发送一次 JSON 不代表接收端一次 `recv` 就能拿到完整 JSON。
- UDP 简单但不保证送达，适合广播发现、低延迟状态上报或可容忍丢包的场景。
- 网络传输中的多字节整数应使用网络字节序，字符串 JSON 本身不需要字节序转换。
- 课程建议使用“4 字节长度 + JSON body”的简单协议，方便演示粘包/拆包。
- `getaddrinfo` 比旧的 `gethostbyname` 更适合现代程序，可以兼容 IPv4/IPv6。
- 任何网络程序都要考虑超时、断开和重连，不要让一次阻塞调用卡死整个程序。

## 操作或演示

1. 查看板端 IP。

```bash
ip addr show
hostname -I
```

2. 编译 TCP Server。

```bash
gcc -Iinclude src/sensor_json.c src/tcp_server.c -o build/sensor_tcp_server
./build/sensor_tcp_server --host 0.0.0.0 --port 9000 --mock
```

3. 使用 PC 客户端连接。

```bash
nc <board-ip> 9000
```

4. 编译课程客户端解析长度头和 JSON。

```bash
gcc pc_client.c -o pc_client
./pc_client <board-ip> 9000
```

5. 演示 UDP 状态包。

```bash
gcc udp_sender.c -o udp_sender
gcc udp_receiver.c -o udp_receiver
./udp_receiver --port 9001
./udp_sender --host <pc-ip> --port 9001
```

## 运行验证

| 验证项 | 命令 | 预期现象 |
| --- | --- | --- |
| 网络连通 | `ping <board-ip>` | PC 与板端互通 |
| 端口监听 | `ss -lntp | grep 9000` | Server 监听 9000 |
| TCP 接收 | `./pc_client <board-ip> 9000` | PC 连续显示传感器 JSON |
| 断开处理 | 关闭 PC 客户端 | Server 不崩溃，可接受下一次连接 |
| 超时处理 | 客户端连接后不读数据 | Server 日志输出超时或断开 |
| UDP 演示 | 运行 sender/receiver | receiver 收到一条状态 JSON |

## 常见问题

| 现象 | 可能原因 | 处理方式 |
| --- | --- | --- |
| PC 连不上板端 | IP 不同网段、防火墙或端口未监听 | 检查 `ip addr`、`ss -lntp`、路由器网络 |
| `Address already in use` | 上次进程仍在或 TIME_WAIT | 检查 `pgrep`，设置 `SO_REUSEADDR` |
| JSON 半截 | TCP 粘包/拆包未处理 | 使用长度头或换行分隔，并循环读取 |
| 程序卡在 `recv` | 未设置超时或非阻塞 | 使用 `setsockopt` 设置 `SO_RCVTIMEO` |

## 本讲成果

- 能解释 TCP/UDP 的差异和适用场景。
- 能编写 TCP Server 并向 PC 客户端发送 JSON 数据。
- 能处理简单封包、字节序、断开和超时。
- 为 HTTP 和 MQTT 服务建立网络调试基础。
