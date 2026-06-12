# Ruyi RISC-V Linux Book

面向 LicheePi 4A（TH1520，RISC-V 64 位）与 RevyOS 的嵌入式 Linux 应用编程课程仓库。课程以 RuyiSDK 为开发入口，从 RISC-V C 语言开发、板级外设、Linux 应用结构、文件与网络编程，逐步推进到 RVV 加速和边缘智能演示。

## 课程主线

本仓库按照 `docs/course-outline.md` 和 `docs/course-plan.md` 组织内容，当前基准为 6 章、24 讲、48 课时。

| 模块 | 章节范围 | 学习重点 |
| --- | --- | --- |
| 基础开发篇 | ch01 | RevyOS 环境、RuyiSDK 工具链、C 工程、GDB、Shell 自动化部署 |
| 板级外设篇 | ch02 | 面包板、GPIO、UART、PWM/ADC/I2C/SPI、DHT22、OLED、温控风扇 |
| Linux 应用篇 | ch03 | 静态库/共享库、进程、IPC、多线程同步、传感器终端骨架 |
| 网络应用篇 | ch04 | 文件 I/O、配置、日志、Socket、HTTP、MQTT 灯光控制 |
| 性能优化篇 | ch05 | RISC-V 向量扩展（RVV）、intrinsics、算法向量化、性能分析 |
| 边缘智能篇 | ch06 | OpenCV、目标检测、TinyML、LLM/VLM 边缘演示 |

## 目录结构

```text
ruyi-riscv-linux-book/
├── README.md          # 项目说明
├── boards/            # 开发板说明和板级差异记录
├── chapters/          # 分章节讲义、实验指导书和代码工程
├── docs/              # 课程大纲、课程计划、制作规范、模板和项目定义
└── slides/            # 授课 PPT
```

## 章节目录

课程当前包含 6 章 24 讲。第二章完成温控风扇阶段项目，第四章完成手机 MQTT 远程灯光控制阶段项目，第六章形成边缘智能综合演示。

| 讲次 | 章节目录 | 模块 | 主要实验/成果 |
| --- | --- | --- | --- |
| 1.1 RISC-V 生态与开发环境搭建 | `ch01_riscv_c_revyos` | 基础开发篇 | LicheePi 4A 镜像烧录、网络配置与 SSH 登录 |
| 1.2 交叉编译工具链与工程构建 | `ch01_riscv_c_revyos` | 基础开发篇 | 交叉编译 Hello World，部署到 LicheePi 4A |
| 1.3 调试技术与版本控制 | `ch01_riscv_c_revyos` | 基础开发篇 | 远程 GDB 调试与 Git 分支实践 |
| 1.4 Shell 编程与自动化部署 | `ch01_riscv_c_revyos` | 基础开发篇 | 一键编译、推送、远程执行和 systemd 服务 |
| 2.1 面包板基础与 GPIO 输出 | `ch02_gpio_uart_peripherals` | 板级外设篇 | 4 路 LED 流水灯 |
| 2.2 GPIO 输入与交互逻辑 | `ch02_gpio_uart_peripherals` | 板级外设篇 | 2 按键 + 4 LED 交互 |
| 2.3 UART 串口通信 | `ch02_gpio_uart_peripherals` | 板级外设篇 | USB-TTL 双向串口命令终端 |
| 2.4 PWM/ADC/I2C/SPI 与综合传感器 | `ch02_gpio_uart_peripherals` | 板级外设篇 | 双温度传感器 + 温控风扇阶段项目 |
| 3.1 静态库与共享库 | `ch03_library_process_thread` | Linux 应用篇 | DHT22 驱动封装为共享库 |
| 3.2 Linux 进程模型与控制 | `ch03_library_process_thread` | Linux 应用篇 | 守护进程化传感器采集 |
| 3.3 进程间通信（IPC） | `ch03_library_process_thread` | Linux 应用篇 | 采集进程到显示进程的数据同步 |
| 3.4 多线程编程与同步 | `ch03_library_process_thread` | Linux 应用篇 | 多线程传感器终端 |
| 4.1 文件 I/O 基础与高级操作 | `ch04_filesystem_network` | 网络应用篇 | mmap 数据文件与 inotify 处理 |
| 4.2 配置管理与日志系统 | `ch04_filesystem_network` | 网络应用篇 | JSON 配置和多级日志系统 |
| 4.3 网络编程基础 | `ch04_filesystem_network` | 网络应用篇 | TCP Server 发送传感器 JSON |
| 4.4 并发服务器、HTTP 与 MQTT 服务 | `ch04_filesystem_network` | 网络应用篇 | 手机 MQTT 远程灯光控制阶段项目 |
| 5.1 RISC-V 向量扩展体系结构 | `ch05_rvv_acceleration` | 性能优化篇 | RVV memcpy/saxpy 初测 |
| 5.2 RVV Intrinsics 编程 | `ch05_rvv_acceleration` | 性能优化篇 | RVV intrinsics 算法片段加速 |
| 5.3 常用算法向量化优化 | `ch05_rvv_acceleration` | 性能优化篇 | RVV SGEMM 或图像算法向量化 |
| 5.4 向量化性能分析与调优 | `ch05_rvv_acceleration` | 性能优化篇 | RVV 性能瓶颈分析报告 |
| 6.1 摄像头接入与 OpenCV 图像处理 | `ch06_ml_edge_ai` | 边缘智能篇 | USB 摄像头 + OpenCV 图像处理 |
| 6.2 目标检测与推理框架 | `ch06_ml_edge_ai` | 边缘智能篇 | NanoDet/ncnn 目标检测 |
| 6.3 TinyML 边缘推理实战 | `ch06_ml_edge_ai` | 边缘智能篇 | 语音关键词唤醒控灯 |
| 6.4 大语言模型与视觉大语言模型 | `ch06_ml_edge_ai` | 边缘智能篇 | LLM/VLM 边缘演示 |

## 推荐学习路径

1. 阅读 [课程大纲](docs/course-outline.md)，确认 6 章 24 讲的知识点、实验和阶段项目。
2. 阅读 [课程计划表](docs/course-plan.md)，确认章节目录、交付物、运行验证和审核节点。
3. 阅读 [课程制作规范 V1.0](docs/course-production-spec-v1.md)，确认讲义、PPT、实验和代码标准。
4. 使用 [课件开发进度和审核表](docs/《RuyiSDK%20RISC-V%20嵌入式编程技术》课件开发进度和审核表.xlsx) 跟踪课程文档、PPT、实验指导书的提交和审核状态。
5. 使用 [课程文档模板](docs/templates/course-document-template.md) 编写每讲 `class*.md`。
6. 使用 [PPT 模板](docs/templates/ppt-template.md) 设计每讲 PPT。
7. 使用 [实验指导书模板](docs/templates/lab-template.md) 编写每讲 `lab/lab*.md` 或阶段项目指导书。
8. 从 `ch01_riscv_c_revyos` 开始，完成 LicheePi 4A + RevyOS 环境、工具链、调试和自动化部署闭环。
9. 继续推进 `ch02_gpio_uart_peripherals`，完成 GPIO、UART、综合传感器和温控风扇项目。
10. 完成 `ch03_library_process_thread` 与 `ch04_filesystem_network`，形成可维护的 Linux 应用结构和 MQTT 灯光控制项目。
11. 进入 `ch05_rvv_acceleration` 和 `ch06_ml_edge_ai`，完成 RVV 性能优化报告与边缘智能演示。
