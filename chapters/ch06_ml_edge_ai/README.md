# 第六章 机器学习

## 本章目标

- 能在 LicheePi 4A + RevyOS 上接入 USB 摄像头，使用 V4L2/OpenCV 完成采集、处理和推流。
- 理解 OpenCV DNN 与 ncnn 的部署流程，能运行轻量目标检测并记录 FPS 和耗时。
- 理解 TinyML KWS 的端到端 pipeline，能将音频采集、MFCC、int8 推理和 GPIO 控制串起来。
- 理解 llama.cpp、量化模型、小型 LLM 和 VLM 的基本部署方式，知道边缘板卡上的资源限制。
- 能在硬件条件不足时选择替代验证路径，例如静态图片、录音文件、预置模型输出或文本 LLM 演示。
- 完成一个边缘智能演示项目，不夸大未经实测的模型性能，所有 FPS、延迟和内存占用都以实测记录为准。

## 前置条件

- 已完成第 1 章的 RevyOS、SSH、编译部署和日志记录能力。
- 建议已完成第 2 章 GPIO 控灯，便于 TinyML KWS 实验联动 LED/继电器。
- 建议已完成第 4 章网络基础，便于理解 MJPEG 推流、Socket 和浏览器验证。
- 已准备 USB 摄像头、USB 麦克风、LED/继电器模块；如缺少硬件，可使用图片文件、视频文件或 WAV 文件替代部分验证。

## 知识简介

本章把前五章的 Linux 应用能力延伸到边缘智能：先解决“能看到”的问题，再解决“能识别”的问题，随后进入语音关键词和轻量大模型。LicheePi 4A 适合作为教学平台展示完整部署链路，但不是高性能 AI 加速卡，因此实验重点是流程、可复现验证和工程取舍，而不是追求夸张帧率或模型规模。

课程默认以 OpenCV 图像处理或 TinyML KWS 作为可稳定演示路径；目标检测、LLM/VLM 作为进阶方向，应根据模型大小、内存、散热和课堂时间选择。

## 环境准备

| 项目 | 要求 | 检查方式 |
| --- | --- | --- |
| 主机环境 | 能下载或准备模型文件，能通过 SSH/SCP 传输到板端 | `scp model.bin sipeed@licheepi-ip:~/ai-labs/` |
| 目标环境 | LicheePi 4A + RevyOS，建议 8GB RAM 版本 | `free -h`、`uname -a` |
| 摄像头 | UVC 免驱 USB 摄像头，默认 `/dev/video0` | `v4l2-ctl --list-devices` |
| 麦克风 | USB 麦克风或声卡，默认 ALSA 设备 | `arecord -l` |
| 软件依赖 | `v4l-utils`、OpenCV、CMake、ncnn、llama.cpp 或预编译二进制 | `pkg-config --modversion opencv4` |
| 部署目录 | 建议使用 `~/ai-labs`，按 `opencv`、`detect`、`kws`、`llm` 分目录 | `mkdir -p ~/ai-labs/{opencv,detect,kws,llm,results}` |

## 学习路径

| 讲次 | 主题 | 学习重点 | 对应实验 |
| --- | --- | --- | --- |
| 6.1 | 摄像头接入与 OpenCV 图像处理 | UVC/V4L2、OpenCV 编译部署、Canny、FPS、MJPEG | USB 摄像头 + Canny + MJPEG |
| 6.2 | 目标检测与推理框架 | OpenCV DNN、ncnn、NanoDet/MobileNet-SSD、量化、后处理 | NanoDet/ncnn 目标检测 |
| 6.3 | TinyML 边缘推理实战 | TFLM、KWS、MFCC、int8 推理、GPIO 控灯 | 语音关键词唤醒控灯 |
| 6.4 | 大语言模型与视觉大语言模型 | llama.cpp、GGUF 量化、小型 LLM、传感器融合、VLM | LLM/VLM 边缘演示 |

## 运行验证

| 验证项 | 预期现象 | 记录 |
| --- | --- | --- |
| 摄像头检测 | 能看到 `/dev/video0` 和支持的分辨率/格式 | `v4l2-ctl --list-formats-ext` |
| OpenCV 处理 | 能保存原图、边缘图或浏览器看到 MJPEG 画面 | 截图、FPS |
| 目标检测 | 能在图片或摄像头帧上画出检测框，并输出耗时 | 模型名、输入尺寸、FPS |
| KWS | 能对 WAV 或麦克风输入输出关键词类别 | 置信度、触发日志 |
| LLM/VLM | 能完成一次文本问答或图像问答演示 | 模型名、量化格式、内存占用 |

## 常见问题

### 摄像头无法打开

现象：`VideoCapture(0)` 返回 false 或 `/dev/video0` 不存在。

原因：摄像头不是 UVC 免驱、供电不足、设备号变化或权限不足。

处理：执行 `lsusb`、`v4l2-ctl --list-devices`，尝试 `/dev/video1`；无法使用摄像头时，用 `sample.mp4` 或静态图片完成算法验证。

### 模型推理速度低于预期

现象：目标检测或 LLM 输出很慢。

原因：模型过大、输入分辨率过高、未量化、内存压力大或后台任务干扰。

处理：降低分辨率，优先使用轻量模型和量化模型；报告中只记录实测 FPS/延迟，不写“实时”这类未经验证的描述。

### 依赖编译耗时过长

现象：OpenCV、ncnn 或 llama.cpp 在板端编译时间很久。

原因：板端性能和存储 I/O 有限。

处理：课堂可使用预编译包或教师提供二进制；学生需记录版本、编译选项和来源，保证实验可复现。

## 本章成果

- 一个 `~/ai-labs` 边缘智能实验目录，包含视觉、检测、KWS、LLM/VLM 的源码或运行脚本。
- 四份实验记录，包含硬件条件、模型路径、命令、输出现象、FPS/延迟/内存占用。
- 一个可现场演示的边缘智能项目，默认可从 OpenCV 图像处理、KWS 控灯、LLM 文本问答中选择其一。
- 可提交材料：源码或脚本、模型说明、运行日志、截图/照片、替代验证说明和待实测数据清单。
