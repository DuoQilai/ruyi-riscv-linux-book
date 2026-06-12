# 6.1 摄像头接入与 OpenCV 图像处理

## 对应大纲

大纲讲次原文：第1讲 摄像头接入与 OpenCV 图像处理。
大纲知识点原文：USB 摄像头驱动与检测；V4L2 C 语言图像采集；OpenCV RISC-V 编译部署；OpenCV 基础图像处理；实时视频流处理；图像保存与网络传输。

USB 摄像头驱动与检测、V4L2 C 语言图像采集、OpenCV RISC-V 编译部署、基础图像处理、实时视频流处理、图像保存与网络传输。

## 目标

学生能用 USB 摄像头采集图像，完成 Canny 边缘检测，并通过 MJPEG 推流到 PC 浏览器；硬件不足时能用图片或视频文件替代验证算法链路。

## 知识点

| # | 知识点 | 内容 |
| --- | --- | --- |
| 1 | USB 摄像头驱动与检测 | UVC、`/dev/video0`、`v4l2-ctl` 查询格式和分辨率。 |
| 2 | V4L2 C 语言图像采集 | `open/ioctl/mmap/STREAMON` 和 YUV/RGB 转换。 |
| 3 | OpenCV RISC-V 编译部署 | 板端安装或源码编译，CMake、libjpeg/libpng 依赖。 |
| 4 | OpenCV 基础图像处理 | `VideoCapture`、`cvtColor`、`GaussianBlur`、`Canny`、`resize`。 |
| 5 | 实时视频流处理 | 帧循环、FPS 计算、ROI、丢帧和延迟。 |
| 6 | 图像保存与网络传输 | JPEG 编码、MJPEG HTTP 响应、PC 浏览器验证。 |

## 讲授要点

- 先确认设备节点和格式，再写 OpenCV 代码；不要把摄像头问题误判成算法问题。
- OpenCV 是课堂的主路径，V4L2 C API 用来解释底层采集流程。
- 实时处理要同时看分辨率、FPS、CPU 占用和延迟。
- MJPEG 推流简单可靠，适合教学演示，但带宽和编码开销较大。
- 没有摄像头时，用静态图片或视频文件完成 Canny 和编码验证，报告中标记为替代验证。

## 操作或演示

```bash
sudo apt update
sudo apt install -y v4l-utils pkg-config libopencv-dev
v4l2-ctl --list-devices
v4l2-ctl -d /dev/video0 --list-formats-ext
pkg-config --modversion opencv4
```

最小采集处理程序的核心流程：

```cpp
cv::VideoCapture cap(0);
cap.set(cv::CAP_PROP_FRAME_WIDTH, 640);
cap.set(cv::CAP_PROP_FRAME_HEIGHT, 480);
while (cap.read(frame)) {
    cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
    cv::GaussianBlur(gray, gray, cv::Size(5, 5), 1.5);
    cv::Canny(gray, edge, 80, 160);
}
```

MJPEG 验证：

```bash
./opencv_canny_mjpeg --device /dev/video0 --width 640 --height 480 --port 8080
```

PC 浏览器访问 `http://licheepi-ip:8080/stream`。

## 运行验证

| 验证项 | 命令或方法 | 预期现象 |
| --- | --- | --- |
| 设备检测 | `v4l2-ctl --list-devices` | 能看到 USB 摄像头和设备节点。 |
| 格式检测 | `v4l2-ctl -d /dev/video0 --list-formats-ext` | 记录 MJPG/YUYV 及分辨率。 |
| OpenCV 采集 | `./opencv_capture --save frame.jpg` | 生成一张清晰图片。 |
| Canny 处理 | `./opencv_canny --input frame.jpg --output edge.jpg` | 输出边缘图。 |
| MJPEG 推流 | 浏览器访问端口 | 能看到连续处理画面和 FPS。 |

## 常见问题

| 现象 | 可能原因 | 处理方式 |
| --- | --- | --- |
| 无 `/dev/video0` | 摄像头未识别或设备号变化 | 查 `lsusb` 和 `v4l2-ctl --list-devices`。 |
| 画面卡顿 | 分辨率过高或 JPEG 编码开销大 | 降到 320x240 或降低帧率。 |
| OpenCV 编译失败 | 依赖缺失或内存不足 | 优先使用系统包，必要时使用教师预编译包。 |

## 本讲成果

- 摄像头检测记录。
- OpenCV Canny 处理程序或替代图片处理程序。
- MJPEG 推流截图、FPS 和 CPU 占用记录。
