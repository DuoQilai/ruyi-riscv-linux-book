# 第四章 文件系统与网络编程

## 本章目标

- 掌握 Linux 文件 I/O、标准 C 文件操作、`mmap`、文件锁和 `inotify`，能把传感器数据可靠落盘。
- 建立配置、日志和工程目录结构意识，使嵌入式应用可配置、可诊断、可维护。
- 掌握 TCP/UDP Socket 基础、字节序、地址解析和超时选项，能在 LicheePi 4A 上提供网络数据服务。
- 完成手机 MQTT 远程灯光控制阶段综合项目：手机发布 JSON 指令，板端通过 GPIO/PWM 控制灯光并回传状态。

## 前置条件

- 已完成第一章的 RevyOS、SSH/SCP、Makefile、GDB、Shell 和 systemd 基础。
- 已完成第二章的 GPIO 输出、PWM 调光、基础外设接线和电路安全。
- 已完成第三章的库封装、守护进程、IPC 和 pthread 同步，能复用传感器或灯光控制模块。
- 具备基本网络知识：IP 地址、端口、客户端/服务端、局域网连通性。

## 知识简介

嵌入式 Linux 应用通常不是孤立地读写一个 GPIO，而是要保存数据、加载配置、记录日志、响应网络请求并把状态反馈给用户。本章把第三章的本地应用骨架扩展到文件系统和网络层：先让数据能落盘和被监控，再让程序通过配置和日志变得可维护，随后提供 TCP 服务，最后用 MQTT 完成手机远程控灯项目。

第 4 章第 4 讲是阶段综合项目，不新增大纲小节，而是在实验指导书中细化 broker、topic、payload、PWM 占空比、权限、断线和调试步骤。

## 环境准备

| 项目 | 要求 | 检查方式 |
| --- | --- | --- |
| 主机环境 | Linux/macOS 主机，能 SSH 到 LicheePi 4A，安装 MQTT 客户端或抓包工具 | `ssh debian@<board-ip> hostname` |
| 目标环境 | LicheePi 4A，运行 RevyOS，接入与手机同一局域网 | `ip addr`、`ping <phone-or-router-ip>` |
| 工具依赖 | `gcc`、`make`、`pkg-config`、`mosquitto`、`mosquitto-clients`、`json-c` 或 `cJSON` | `mosquitto -h`、`mosquitto_pub -h` |
| 硬件连接 | LED 灯光模块或 MOSFET/PWM 调光模块，连接课程指定 GPIO/PWM | 用第二章 GPIO/PWM 示例确认开关和调光 |
| 手机端 | 安装 MQTT 客户端 App，能配置 broker、topic 和 JSON payload | 手机订阅测试 topic 能收到板端消息 |

## 学习路径

| 讲次 | 主题 | 学习重点 | 对应实验 |
| --- | --- | --- | --- |
| 4.1 | 文件 I/O 基础与高级操作 | POSIX I/O、stdio、stat、mmap、文件锁、inotify | mmap 数据文件 + inotify 处理 |
| 4.2 | 配置管理与日志系统 | getopt、INI、JSON、日志级别、日志轮转、目录结构 | JSON 配置 + 多级日志系统 |
| 4.3 | 网络编程基础 | TCP、UDP、封包、getaddrinfo、setsockopt、超时 | LicheePi 4A 作为 TCP Server 发送传感器 JSON |
| 4.4 | 并发服务器、HTTP 与 MQTT 服务 | 并发模型、epoll、HTTP、mosquitto、topic、JSON、状态回传 | 手机 MQTT 远程灯光控制阶段项目 |

## 运行验证

| 验证项 | 预期现象 | 记录 |
| --- | --- | --- |
| 文件落盘 | 传感器数据写入 mmap 数据文件，inotify 能检测修改 | 记录文件路径和事件输出 |
| 配置日志 | 修改 JSON 配置后采样频率、阈值或日志级别变化 | 记录配置内容和日志输出 |
| TCP 通信 | PC 客户端连接板端 TCP Server 并收到 JSON 数据 | 记录 IP、端口和样例 JSON |
| MQTT 控灯 | 手机发布开关和亮度指令，灯光模块响应，板端发布状态 | 记录 broker、topic、payload 和状态 topic |
| 断线恢复 | broker 停止或网络断开后，板端能重连或输出明确错误 | 记录断线时间和恢复现象 |

## 本章成果

- 一个基于 mmap 与 inotify 的本地数据文件示例。
- 一个支持 JSON 配置、多级日志和目录结构规范的传感器终端工程。
- 一个 TCP Server，可向 PC 客户端发送传感器 JSON 数据。
- 一个 MQTT 远程灯光控制阶段项目，包含 broker 配置、手机端操作、板端 GPIO/PWM 控制、状态回传和故障排查记录。
- 可提交材料：源码、配置文件、运行命令、网络拓扑、手机端截图、硬件照片和验收记录。
