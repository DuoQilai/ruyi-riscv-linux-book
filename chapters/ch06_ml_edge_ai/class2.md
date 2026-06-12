# 6.2 目标检测与推理框架

## 对应大纲

大纲讲次原文：第2讲 目标检测与推理框架。
大纲知识点原文：目标检测算法概述；OpenCV DNN 模块；ncnn 推理框架；轻量级检测模型部署；模型量化与优化；实时人脸检测实战。

目标检测算法概述、OpenCV DNN 模块、ncnn 推理框架、轻量级检测模型部署、模型量化与优化、实时人脸检测实战。

## 目标

学生能理解轻量目标检测的部署流程，使用 OpenCV DNN 或 ncnn 在 LicheePi 4A 上完成图片/摄像头帧检测，并记录模型、输入尺寸、耗时和 FPS。

## 知识点

| # | 知识点 | 内容 |
| --- | --- | --- |
| 1 | 目标检测算法概述 | 两阶段/单阶段、YOLO 演进、mAP、NMS。 |
| 2 | OpenCV DNN 模块 | `readNet`、`blobFromImage`、`forward` 和后处理。 |
| 3 | ncnn 推理框架 | RISC-V 编译、param/bin、输入归一化、输出解析。 |
| 4 | 轻量级检测模型部署 | NanoDet、MobileNet-SSD、输入尺寸和类别标签。 |
| 5 | 模型量化与优化 | FP32/FP16/INT8、校准、精度损失和速度取舍。 |
| 6 | 实时人脸检测实战 | Haar Cascade 作为低门槛替代路径，检测框绘制和 FPS 优化。 |

## 讲授要点

- 目标检测不是只有推理，预处理、后处理和绘制同样会影响速度。
- ncnn 更适合轻量端侧部署，OpenCV DNN 更适合快速验证模型流程。
- 输入分辨率直接影响速度和召回率，实验必须记录输入尺寸。
- 人脸 Haar 检测可以作为模型条件不足时的替代验证，但要说明它不是现代通用目标检测模型。
- 所有“实时”结论都需要 FPS 实测支撑。

## 操作或演示

```bash
mkdir -p ~/ai-labs/detect/{models,results}
cd ~/ai-labs/detect
```

OpenCV DNN 图片检测示例：

```bash
./detect_opencv_dnn \
  --model models/mobilenet-ssd.onnx \
  --labels models/labels.txt \
  --input test.jpg \
  --output results/test_detect.jpg
```

ncnn 摄像头检测示例：

```bash
./detect_ncnn \
  --param models/nanodet.param \
  --bin models/nanodet.bin \
  --labels models/coco.txt \
  --camera /dev/video0 \
  --size 320 \
  --repeat 100
```

替代验证：

```bash
./detect_haar_face --input test.jpg --output results/face.jpg
```

## 运行验证

| 验证项 | 命令或方法 | 预期现象 |
| --- | --- | --- |
| 模型加载 | 启动检测程序 | 不报模型/标签文件缺失。 |
| 图片检测 | `--input test.jpg` | 输出带检测框图片。 |
| 摄像头检测 | `--camera /dev/video0` | 画面显示检测框和 FPS。 |
| 性能记录 | `--repeat 100` | 输出平均预处理、推理、后处理耗时。 |

## 常见问题

| 现象 | 可能原因 | 处理方式 |
| --- | --- | --- |
| 检测框错位 | resize/letterbox 后坐标还原错误 | 记录缩放比例和 padding，统一坐标系。 |
| 模型加载失败 | param/bin/labels 不匹配 | 检查模型来源和类别数量。 |
| FPS 很低 | 模型过大或输入尺寸过高 | 改用 320 输入、轻量模型或图片离线验证。 |

## 本讲成果

- 一次图片或摄像头目标检测演示。
- 一份模型信息记录：模型名、格式、输入尺寸、类别数、量化方式。
- 一份 FPS/耗时表，注明是否为摄像头实时或图片离线验证。
