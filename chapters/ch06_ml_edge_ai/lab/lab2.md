# 实验 6.2：NanoDet/ncnn 目标检测

## 对应讲次

| 项目 | 内容 |
| --- | --- |
| 章节 | 第 6 章 机器学习 |
| 讲次 | 第 2 讲 目标检测与推理框架 |
| 课程主题 | OpenCV DNN、ncnn、轻量检测、FPS 记录 |
| 实验类型 | 必做实验 |

大纲讲次原文：第2讲 目标检测与推理框架。
大纲实验原文：在 LicheePi 4A 上部署 NanoDet（ncnn），USB 摄像头实时目标检测并显示 FPS
大纲知识点原文：目标检测算法概述；OpenCV DNN 模块；ncnn 推理框架；轻量级检测模型部署；模型量化与优化；实时人脸检测实战。

## 实验目标

- 准备 ncnn 检测程序、模型 `param/bin` 和标签文件。
- 对静态图片完成一次检测并输出带框图片。
- 条件允许时接入 USB 摄像头，显示检测框和 FPS；条件不足时提交图片离线检测。

## 对应知识点

| # | 知识点 | 本实验中的验证方式 |
| --- | --- | --- |
| 1 | 目标检测算法概述 | 记录模型类别、输入尺寸和 NMS 阈值。 |
| 2 | OpenCV DNN 模块 | 可选使用 ONNX 做对照验证。 |
| 3 | ncnn 推理框架 | 加载 `param/bin` 并运行推理。 |
| 4 | 轻量级检测模型部署 | 使用 NanoDet 或 MobileNet-SSD。 |
| 5 | 模型量化与优化 | 记录 FP32/INT8 或实际模型格式。 |
| 6 | 实时人脸检测实战 | 条件不足时用 Haar 检测替代。 |

## 实验环境

| 项目 | 要求 |
| --- | --- |
| 主机环境 | 可准备模型并传输到板端 |
| 目标板 | LicheePi 4A |
| 目标系统 | RevyOS |
| 硬件连接 | USB 摄像头可选 |
| 软件依赖 | ncnn、OpenCV、`g++`、`cmake` |

## 硬件连接或部署关系

| 模块 | 引脚/接口 | 连接说明 | 注意事项 |
| --- | --- | --- | --- |
| USB 摄像头 | USB | 用于实时检测 | 可用静态图片替代 |
| 模型文件 | 文件系统 | `~/ai-labs/detect/models` | 确保 param/bin/labels 匹配 |

## 实验任务

### 任务 1：准备模型

将 `nanodet.param`、`nanodet.bin`、`coco.txt` 放入 `models/`，记录模型来源和输入尺寸。

### 任务 2：图片检测

对 `test.jpg` 运行检测，输出 `results/test_detect.jpg`。

### 任务 3：摄像头检测

使用 `/dev/video0` 连续检测，记录平均推理耗时和 FPS。

## 实验步骤

```bash
mkdir -p ~/ai-labs/detect/{models,results}
cd ~/ai-labs/detect
ls -lh models | tee results/model_files.txt
```

```bash
./detect_ncnn \
  --param models/nanodet.param \
  --bin models/nanodet.bin \
  --labels models/coco.txt \
  --input test.jpg \
  --output results/test_detect.jpg \
  --size 320 | tee results/image_detect.txt
```

摄像头路径：

```bash
./detect_ncnn \
  --param models/nanodet.param \
  --bin models/nanodet.bin \
  --labels models/coco.txt \
  --camera /dev/video0 \
  --size 320 \
  --repeat 100 | tee results/camera_detect.txt
```

替代验证：

```bash
./detect_haar_face --input test.jpg --output results/haar_face.jpg \
  | tee results/haar_face.txt
```

## 运行验证

| 验证项 | 预期现象 | 是否通过 |
| --- | --- | --- |
| 模型加载 | 程序不报 param/bin 缺失 |  |
| 图片检测 | 输出带框图片 |  |
| 摄像头检测 | 显示检测框和 FPS |  |
| 性能记录 | 输出预处理、推理、后处理耗时 |  |

## 验收记录

| 记录项 | 内容 |
| --- | --- |
| 运行命令 | `./detect_ncnn --param ... --camera /dev/video0 --size 320 --repeat 100` |
| 关键输出 | 检测类别、置信度、FPS、平均耗时 |
| 截图或照片 | 带框图片或实时画面截图 |
| 异常处理 | 模型不可用时提交 Haar 或图片离线验证 |

## 常见问题

| 现象 | 可能原因 | 处理方式 |
| --- | --- | --- |
| 无检测框 | 阈值太高或模型标签不匹配 | 降低阈值并检查 labels。 |
| 框位置不准 | letterbox 坐标还原错误 | 检查缩放和 padding。 |
| 实时速度慢 | 模型或输入过大 | 改用 320 输入，记录真实 FPS。 |

## 提交要求

- 实验记录：模型名、输入尺寸、阈值、运行路径。
- 运行截图：带框图片或摄像头检测画面。
- 源码或配置文件：检测程序、模型文件清单。
- 简短说明：实测 FPS，不写未经验证的“实时”结论。
