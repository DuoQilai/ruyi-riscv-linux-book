# Slides

存放新版 6 章、24 讲课程配套 PPT 或导出 PDF。

## 交付口径

- 课程基准：`docs/course-outline.md` 与 `docs/course-plan.md`。
- 目标平台：LicheePi 4A。
- 目标系统：RevyOS。
- 交付数量：6 章 × 4 讲 = 24 份授课 PPT。
- 每讲 PPT 应与对应课程文档、实验或项目指导书同步命名、同步更新。
- 第 2 章章末项目是温控风扇，第 4 章章末项目是手机 MQTT 远程灯光控制，第 6.3 是 TinyML KWS。

## 建议命名

| 章节 | 讲次 | 建议文件名 |
| --- | --- | --- |
| ch01 | 1.1 RISC-V 生态与开发环境搭建 | `ch01_01_riscv_ecosystem_revyos.pptx` |
| ch01 | 1.2 交叉编译工具链与工程构建 | `ch01_02_toolchain_build.pptx` |
| ch01 | 1.3 调试技术与版本控制 | `ch01_03_debug_git.pptx` |
| ch01 | 1.4 Shell 编程与自动化部署 | `ch01_04_shell_deploy.pptx` |
| ch02 | 2.1 面包板基础与 GPIO 输出 | `ch02_01_breadboard_gpio_output.pptx` |
| ch02 | 2.2 GPIO 输入与交互逻辑 | `ch02_02_gpio_input_interaction.pptx` |
| ch02 | 2.3 UART 串口通信 | `ch02_03_uart_serial.pptx` |
| ch02 | 2.4 PWM/ADC/I2C/SPI 与综合传感器 | `ch02_04_peripherals_fan_project.pptx` |
| ch03 | 3.1 静态库与共享库 | `ch03_01_static_shared_library.pptx` |
| ch03 | 3.2 Linux 进程模型与控制 | `ch03_02_process_control.pptx` |
| ch03 | 3.3 进程间通信 | `ch03_03_ipc.pptx` |
| ch03 | 3.4 多线程编程与同步 | `ch03_04_pthread_sync.pptx` |
| ch04 | 4.1 文件 I/O 基础与高级操作 | `ch04_01_file_io.pptx` |
| ch04 | 4.2 配置管理与日志系统 | `ch04_02_config_logging.pptx` |
| ch04 | 4.3 网络编程基础 | `ch04_03_socket_network.pptx` |
| ch04 | 4.4 并发服务器、HTTP 与 MQTT 服务 | `ch04_04_http_mqtt_light.pptx` |
| ch05 | 5.1 RISC-V 向量扩展体系结构 | `ch05_01_rvv_arch.pptx` |
| ch05 | 5.2 RVV Intrinsics 编程 | `ch05_02_rvv_intrinsics.pptx` |
| ch05 | 5.3 常用算法向量化优化 | `ch05_03_algorithm_vectorization.pptx` |
| ch05 | 5.4 向量化性能分析与调优 | `ch05_04_rvv_performance.pptx` |
| ch06 | 6.1 摄像头接入与 OpenCV 图像处理 | `ch06_01_camera_opencv.pptx` |
| ch06 | 6.2 目标检测与推理框架 | `ch06_02_object_detection.pptx` |
| ch06 | 6.3 TinyML 边缘推理实战 | `ch06_03_tinyml_kws.pptx` |
| ch06 | 6.4 大语言模型与视觉大语言模型 | `ch06_04_llm_vlm.pptx` |

## 当前课件

- `ch03_gpio_class2_gpio_output_led.pptx`：旧命名课件，需按新版 24 讲口径评估是否迁移到 `ch02_01_breadboard_gpio_output.pptx` 或拆分重做。
