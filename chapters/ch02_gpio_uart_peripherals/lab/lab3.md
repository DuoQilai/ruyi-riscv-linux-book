# 实验 2.3：USB-TTL 双向串口终端

## 对应讲次

| 项目 | 内容 |
| --- | --- |
| 章节 | 第 2 章 GPIO、UART、常用外设 |
| 讲次 | 第 3 讲 |
| 课程主题 | UART 串口通信 |
| 实验类型 | 必做实验 |

大纲讲次原文：第3讲 UART 串口通信。
大纲实验原文：USB-TTL 模块连接 LicheePi 4A UART，编写双向串口通信程序，实现简易命令终端
大纲知识点原文：UART 通信协议基础；LicheePi 4A UART 引脚识别；termios 串口配置；串口收发程序；环形缓冲区设计；简易串口调试终端。

## 实验目标

- 使用 USB-TTL 模块连接 LicheePi 4A UART。
- 使用 termios 配置 115200 8N1 串口参数。
- 编写双向收发程序，支持回显和换行分隔命令。
- 实现 `help`、`status`、`echo`、`led` 等简易命令。

## 对应知识点

| # | 知识点 | 本实验中的验证方式 |
| --- | --- | --- |
| 1 | UART 通信协议基础 | 统一波特率、数据位、校验位、停止位 |
| 2 | LicheePi 4A UART 引脚识别 | 记录 TX/RX/GND 和 `/dev/tty*` 设备节点 |
| 3 | termios 串口配置 | 程序中设置 115200 8N1、关闭流控 |
| 4 | 串口收发程序 | 主机输入命令，板端读取并返回响应 |
| 5 | 环形缓冲区设计 | 分多次输入长命令仍能正确拼接 |
| 6 | 简易串口调试终端 | `help/status/echo` 命令可用 |

## 实验环境

| 项目 | 要求 |
| --- | --- |
| 主机环境 | Linux/macOS/Windows 均可，需串口终端工具；编译仍建议 Linux 主机 |
| 目标板 | LicheePi 4A |
| 目标系统 | RevyOS |
| 硬件连接 | USB-TTL 模块、杜邦线，3.3V TTL 电平 |
| 软件依赖 | 板端串口设备，主机 `picocom`/`minicom`/其他串口工具 |

## 硬件连接或部署关系

| 模块 | 引脚/接口 | 连接说明 | 注意事项 |
| --- | --- | --- | --- |
| USB-TTL GND | LicheePi 4A GND | 共地 | 必须连接 |
| USB-TTL TXD | LicheePi 4A UART RX | 交叉连接 | 不接 5V 电源到信号脚 |
| USB-TTL RXD | LicheePi 4A UART TX | 交叉连接 | 使用 3.3V TTL |
| 主机串口工具 | `/dev/ttyUSB0` 等 | 连接 USB-TTL | 设备名以实际系统为准 |

## 实验任务

### 任务 1：确认串口设备

主机和板端分别确认串口设备节点，记录波特率和连接方式。

### 任务 2：实现串口收发

板端程序使用 `open`、`tcgetattr`、`tcsetattr`、`read`、`write` 完成基本收发。

### 任务 3：实现命令终端

接收以 `\n` 结尾的命令，解析并返回结果。输入错误命令时返回提示。

## 实验步骤

1. 主机端查看 USB-TTL 设备。

```bash
ls -l /dev/ttyUSB* /dev/tty.usbserial* 2>/dev/null
picocom -b 115200 /dev/ttyUSB0
```

2. 板端确认 UART 设备。

```bash
ls -l /dev/ttyS* /dev/ttyTHS* 2>/dev/null
stty -F /dev/ttyS0 -a
```

3. 编译并部署程序。

```bash
make clean
make TARGET=uart_console
scp build/uart_console <user>@<board-ip>:/tmp/uart_console
```

4. 板端运行串口终端。

```bash
ssh <user>@<board-ip> 'chmod +x /tmp/uart_console'
ssh <user>@<board-ip> '/tmp/uart_console --dev /dev/ttyS0 --baud 115200'
```

5. 主机串口终端输入命令。

```text
help
status
echo hello revyos
led chase
unknown
```

## 运行验证

| 验证项 | 预期现象 | 是否通过 |
| --- | --- | --- |
| 串口设备 | 主机能看到 USB-TTL 设备节点 |  |
| 参数一致 | 两端均为 115200 8N1 |  |
| 回显 | 输入文本后板端返回响应 |  |
| 命令解析 | `help/status/echo` 返回正确内容 |  |
| 异常命令 | 未知命令返回错误提示但程序不退出 |  |

## 验收记录

| 记录项 | 内容 |
| --- | --- |
| 运行命令 | `picocom`、`stty`、`/tmp/uart_console ...` |
| 关键输出 | 串口设备名、命令响应、错误命令提示 |
| 截图或照片 | TX/RX 接线、串口终端输出 |
| 异常处理 | 记录乱码、无输出、阻塞等问题 |

## 常见问题

| 现象 | 可能原因 | 处理方式 |
| --- | --- | --- |
| 输出乱码 | 波特率或 8N1 参数不一致 | 两端统一为 115200 8N1 |
| 无法收发 | TX/RX 未交叉或未共地 | 断电后检查三根线 |
| 设备被占用 | 串口已被登录终端或其他程序占用 | 关闭占用程序，换空闲 UART |
| 长命令丢字符 | 未使用缓冲或缓冲区太小 | 使用环形缓冲，增加溢出处理 |

## 提交要求

- 实验记录：UART 引脚、设备节点、串口参数。
- 运行截图：串口终端命令和响应。
- 源码或配置文件：termios 初始化、环形缓冲、命令解析源码。
- 简短说明：说明字节流如何被解析成命令。
