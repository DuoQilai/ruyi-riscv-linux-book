# 第二章 GPIO、UART、常用外设

本章从面包板和电路安全开始，逐步完成 GPIO 输出、GPIO 输入、UART 通信、PWM/ADC/I2C/SPI 认知，并在章末整合 DHT22 x2、OLED 和风扇驱动，形成“温控风扇”阶段综合项目。目标平台统一为 LicheePi 4A + RevyOS，用户态外设访问优先使用 Linux 标准接口和 gpiod。

## 本章目标

- 能安全搭建面包板电路，理解限流、电平、共地和外部供电的基本要求。
- 能使用 sysfs/gpiod 控制 GPIO 输出，实现 4 路 LED Blink 和流水灯。
- 能读取 GPIO 输入，完成按键消抖、边沿事件、长按/短按/双击识别。
- 能使用 termios 编写 UART 收发程序和简易命令终端。
- 能理解 PWM、ADC、I2C、SPI 的 Linux 用户态访问方式。
- 能完成双温度传感器 + 温控风扇项目，并通过 OLED/串口显示状态。

## 前置条件

- 已完成第 1 章，能够 SSH 登录 LicheePi 4A，能交叉编译和部署 C 程序。
- 具备基本 C 语言结构体、数组、函数和文件 I/O 能力。
- 准备面包板、LED、限流电阻、按键、USB-TTL、DHT22 x2、SSD1306 OLED、风扇、继电器模块或 MOSFET 驱动模块、杜邦线和独立电源。
- 实验前确认 LicheePi 4A 40pin 引脚图、GPIO 编号映射和所有外设电平要求。

## 知识简介

嵌入式 Linux 外设编程不是直接操作裸机寄存器，而是通过内核暴露的设备节点、字符设备、sysfs 或专用子系统访问硬件。本章先用 LED 和按键建立 GPIO 输入输出直觉，再用 UART 建立串行通信能力，最后把 PWM、I2C、DHT22 和风扇控制整合成闭环系统。章末项目会成为后续库封装、进程线程、配置日志和网络上报的基础素材。

## 环境准备

| 项目 | 要求 | 检查方式 |
| --- | --- | --- |
| 主机环境 | 已有 RISC-V 交叉编译工具链、Makefile 工程模板、SSH/SCP | `riscv64-unknown-linux-gnu-gcc --version` |
| 目标环境 | LicheePi 4A + RevyOS，可安装 `gpiod`、`i2c-tools`、`minicom` 或 `picocom` | `cat /etc/os-release`、`gpioinfo` |
| 工具依赖 | `libgpiod-dev`、`gpiod`、`i2c-tools`、`strace`、串口终端工具 | `gpioinfo`、`i2cdetect -l` |
| 硬件连接 | 面包板、共地、3.3V 信号、风扇独立供电、OLED I2C、USB-TTL | 万用表测通断，逐路上电验证 |

## 学习路径

| 讲次 | 主题 | 学习重点 | 配套实验 |
| --- | --- | --- | --- |
| 2.1 | 面包板基础与 GPIO 输出 | 接线安全、GPIO 编号、gpiod 输出、LED 流水灯 | 4 路 LED 流水灯 |
| 2.2 | GPIO 输入与交互逻辑 | 按键输入、边沿事件、消抖、长短按和状态机 | 2 按键 + 4 LED 交互 |
| 2.3 | UART 串口通信 | UART 协议、termios、select、环形缓冲、命令解析 | USB-TTL 双向串口终端 |
| 2.4 | PWM/ADC/I2C/SPI 与综合传感器 | PWM、ADC、I2C、OLED、DHT22、温控风扇 | 双温度传感器 + 温控风扇 |

## 运行验证

| 验证项 | 预期现象 | 记录 |
| --- | --- | --- |
| GPIO 输出 | 4 路 LED 按轮流、来回或随机模式点亮 | 记录 GPIO chip/line 和接线照片 |
| GPIO 输入 | 单击、长按、双击能稳定触发 LED 状态变化 | 记录事件日志和消抖参数 |
| UART 通信 | USB-TTL 能收发命令，板端回显并响应 `help/status` | 记录设备节点、波特率和输出 |
| I2C/OLED | `i2cdetect` 能看到 OLED 地址，屏幕显示状态 | 记录总线号和地址 |
| 温控风扇 | DHT22 x2 读数有效，温度高于阈值启动风扇，低于阈值减回差后停止 | 记录阈值、回差、异常读数和风扇状态 |

## 本章成果

- 一套安全可靠的 LicheePi 4A 面包板实验接线记录。
- 4 路 LED 和 2 按键交互程序，具备 gpiod 输入输出基础。
- 一个 UART 命令终端，可用于后续调试和状态输出。
- 一个双 DHT22 + 风扇/继电器或 MOSFET + OLED/串口显示的阶段综合项目。

