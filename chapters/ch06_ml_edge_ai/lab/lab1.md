# 实验 6.1：USB 摄像头 + Canny + MJPEG

## 对应讲次

| 项目 | 内容 |
| --- | --- |
| 章节 | 第 6 章 机器学习 |
| 讲次 | 第 1 讲 摄像头接入与 OpenCV 图像处理 |
| 课程主题 | USB 摄像头、OpenCV、Canny、MJPEG 推流 |
| 实验类型 | 必做实验 |

大纲讲次原文：第1讲 摄像头接入与 OpenCV 图像处理。
大纲实验原文：USB 摄像头连接 LicheePi 4A，OpenCV 实时采集+Canny 边缘检测，MJPEG 推流到 PC 浏览器
大纲知识点原文：USB 摄像头驱动与检测；V4L2 C 语言图像采集；OpenCV RISC-V 编译部署；OpenCV 基础图像处理；实时视频流处理；图像保存与网络传输。

## 实验目标

- 检测 UVC 摄像头设备节点、格式和分辨率。
- 编写 OpenCV 程序完成采集、灰度化、模糊、Canny 边缘检测。
- 通过 MJPEG HTTP 服务在 PC 浏览器查看处理后画面。

## 对应知识点

| # | 知识点 | 本实验中的验证方式 |
| --- | --- | --- |
| 1 | USB 摄像头驱动与检测 | 使用 `v4l2-ctl` 查询设备。 |
| 2 | V4L2 C 语言图像采集 | 了解 OpenCV 底层设备节点来源。 |
| 3 | OpenCV RISC-V 编译部署 | 检查 `opencv4` 包和编译示例。 |
| 4 | OpenCV 基础图像处理 | 执行 Canny 处理。 |
| 5 | 实时视频流处理 | 统计 FPS。 |
| 6 | 图像保存与网络传输 | MJPEG 推流到浏览器。 |

## 实验环境

| 项目 | 要求 |
| --- | --- |
| 主机环境 | PC 与 LicheePi 4A 在同一网络，可访问板端端口 |
| 目标板 | LicheePi 4A |
| 目标系统 | RevyOS |
| 硬件连接 | UVC USB 摄像头连接到板端 USB 口 |
| 软件依赖 | `v4l-utils`、`libopencv-dev`、`g++`、`cmake` |

## 硬件连接或部署关系

| 模块 | 引脚/接口 | 连接说明 | 注意事项 |
| --- | --- | --- | --- |
| USB 摄像头 | USB-A/转接线 | 插入 LicheePi 4A USB 口 | 供电不足时使用带供电 HUB |
| PC 浏览器 | 局域网 | 访问 `http://板端IP:8080/stream` | 防火墙需允许访问 |

## 实验任务

### 任务 1：检测摄像头

确认 `/dev/video0` 或实际设备号，记录支持格式和分辨率。

### 任务 2：运行 Canny

采集图像并输出边缘检测画面，记录分辨率和 FPS。

### 任务 3：MJPEG 推流

启动 HTTP 服务，在 PC 浏览器查看连续画面并截图。

## 实验步骤

```bash
sudo apt update
sudo apt install -y v4l-utils libopencv-dev g++ cmake
mkdir -p ~/ai-labs/opencv/results
cd ~/ai-labs/opencv
v4l2-ctl --list-devices | tee results/camera_devices.txt
v4l2-ctl -d /dev/video0 --list-formats-ext | tee results/camera_formats.txt
```

```bash
g++ opencv_canny_mjpeg.cpp -O2 -o opencv_canny_mjpeg $(pkg-config --cflags --libs opencv4)
./opencv_canny_mjpeg --device /dev/video0 --width 640 --height 480 --port 8080 \
  | tee results/mjpeg_run.txt
```

替代验证：没有摄像头时，使用图片或视频文件。

```bash
./opencv_canny_mjpeg --input sample.mp4 --port 8080
./opencv_canny_image --input sample.jpg --output results/edge.jpg
```

## 运行验证

| 验证项 | 预期现象 | 是否通过 |
| --- | --- | --- |
| 摄像头检测 | `v4l2-ctl` 能列出设备和格式 |  |
| 程序启动 | 终端输出监听端口和 FPS |  |
| 浏览器访问 | PC 端看到边缘检测画面 |  |
| 替代验证 | 无摄像头时生成 `edge.jpg` |  |

## 验收记录

| 记录项 | 内容 |
| --- | --- |
| 运行命令 | `./opencv_canny_mjpeg --device /dev/video0 --width 640 --height 480 --port 8080` |
| 关键输出 | 分辨率、FPS、CPU 占用 |
| 截图或照片 | 浏览器画面截图、摄像头连接照片 |
| 异常处理 | 记录摄像头不可用时的图片/视频替代验证 |

## 常见问题

| 现象 | 可能原因 | 处理方式 |
| --- | --- | --- |
| 摄像头打不开 | 设备号变化或权限不足 | 尝试 `/dev/video1`，检查 `v4l2-ctl`。 |
| 浏览器无法访问 | IP/端口错误或防火墙 | 用 `ip addr` 确认板端 IP。 |
| FPS 很低 | 分辨率过高或编码开销大 | 改为 320x240 或降低 JPEG 质量。 |

## 提交要求

- 实验记录：设备节点、格式、分辨率、FPS。
- 运行截图：浏览器 MJPEG 画面或 `edge.jpg`。
- 源码或配置文件：OpenCV 程序、编译命令。
- 简短说明：是否使用替代验证。
