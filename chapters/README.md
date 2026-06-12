# Chapters

本目录已经切换到新版 6 章结构。旧版章节目录已删除，后续内容维护只以新版 6 章目录为准。

新版课程以 `docs/course-outline.md` 和 `docs/course-plan.md` 为准，共 6 章、24 讲、24 个实验/项目。后续新增与整理内容应写入新版目录。

## 新版章节目录

| 章节目录 | 二级章节 | 本章目标 | 核心内容 |
| --- | --- | --- | --- |
| `ch01_riscv_c_revyos` | RISC-V C语言开发与调试 | 建立 RevyOS 开发环境、工具链和基础调试能力 | 按新版大纲整理 RevyOS、工具链、Hello World、ELF 观察和 Makefile 内容 |
| `ch02_gpio_uart_peripherals` | GPIO、UART、常用外设 | 建立面包板接线、GPIO/UART/PWM/ADC/I2C/SPI 和传感控制能力 | 按新版大纲整理 GPIO、UART、PWM/ADC、I2C/SPI、传感器和温控风扇项目 |
| `ch03_library_process_thread` | 程序库、进程与线程 | 建立库封装、进程控制、IPC 和多线程同步能力 | 按新版大纲补写库封装、进程、IPC、线程和同步内容 |
| `ch04_filesystem_network` | 文件系统与网络编程 | 建立文件、配置、日志、Socket、HTTP 和 MQTT 能力 | 按新版大纲整理文件系统、网络编程、mosquitto 和 MQTT 灯光项目 |
| `ch05_rvv_acceleration` | RISC-V 向量扩展（RVV）加速编程 | 建立 RVV 架构、intrinsics、算法向量化和性能分析能力 | RVV 架构、intrinsics、SGEMM、perf 性能分析和加速比记录 |
| `ch06_ml_edge_ai` | 机器学习 | 建立 OpenCV、目标检测、TinyML 和 LLM/VLM 部署认知 | 摄像头/OpenCV、ncnn 目标检测、TinyML KWS、LLM/VLM 边缘演示 |

## 新版章节内部结构

每章固定包含 4 讲和 4 个实验/项目：

```text
chXX_topic/
├── README.md
├── class1.md
├── class2.md
├── class3.md
├── class4.md
└── lab/
    ├── lab1.md
    ├── lab2.md
    ├── lab3.md
    └── lab4.md
```

## 维护规则

- 新版目录是后续维护的主线，章节名、讲次和实验目标必须与 `docs/course-outline.md` 一致。
- 每个讲次文档应覆盖对应大纲中的 6 个知识点，并给出课堂演示、运行验证和本讲成果。
- 每个实验指导书应说明实验目标、环境、连接或部署关系、操作步骤、运行验证和提交要求。
- 第 2 章章末项目为“双温度传感器 + 温控风扇”；第 4 章章末项目为“手机 MQTT 远程灯光控制”；第 6 章形成边缘智能演示项目。
