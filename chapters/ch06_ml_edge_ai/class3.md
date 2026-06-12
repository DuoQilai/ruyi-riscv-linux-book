# 6.3 TinyML 边缘推理实战

## 对应大纲

大纲讲次原文：第3讲 TinyML 边缘推理实战。
大纲知识点原文：TinyML 概念与适用场景；TensorFlow Lite Micro 编译；模型训练与导出；语音特征提取（MFCC）；int8 推理引擎集成；关键词唤醒控灯系统。

TinyML 概念与适用场景、TensorFlow Lite Micro 编译、模型训练与导出、语音特征提取（MFCC）、int8 推理引擎集成、关键词唤醒控灯系统。

## 目标

学生能理解 KWS 的端到端流程，使用 USB 麦克风或 WAV 文件完成“开灯/关灯”关键词识别，并在条件允许时联动 GPIO 控制 LED/继电器。

## 知识点

| # | 知识点 | 内容 |
| --- | --- | --- |
| 1 | TinyML 概念与适用场景 | 小模型、低功耗、低延迟和端侧隐私。 |
| 2 | TensorFlow Lite Micro 编译 | TFLM 移植、resolver、arena 内存和算子裁剪。 |
| 3 | 模型训练与导出 | TF 到 TFLite，再转 C array 或模型文件。 |
| 4 | 语音特征提取（MFCC） | 采样、分帧、窗函数、FFT、Mel 滤波和 DCT。 |
| 5 | int8 推理引擎集成 | scale/zero-point、量化张量、Conv/Depthwise/FC。 |
| 6 | 关键词唤醒控灯系统 | 麦克风采集、滑动窗口、阈值判断、GPIO 控制。 |

## 讲授要点

- KWS 的准确率很依赖数据集、麦克风位置和环境噪声，课堂不承诺固定识别率。
- TinyML 重点是“小而可控”，不是把大型语音模型硬塞到板端。
- MFCC 是从音频波形到模型输入的关键桥梁，调试时要能保存中间特征。
- GPIO 控灯要复用第 2 章安全接线方法，继电器或风扇负载必须单独说明供电。
- 硬件不足时，WAV 文件离线识别是有效替代验证。

## 操作或演示

检查音频设备：

```bash
arecord -l
arecord -D plughw:0,0 -f S16_LE -r 16000 -c 1 -d 3 sample.wav
aplay sample.wav
```

WAV 离线推理：

```bash
./kws_infer --model models/kws_int8.tflite --wav sample_open.wav
./kws_infer --model models/kws_int8.tflite --wav sample_close.wav
```

麦克风实时推理并联动 GPIO：

```bash
sudo ./kws_light \
  --model models/kws_int8.tflite \
  --device plughw:0,0 \
  --gpiochip gpiochip0 \
  --line 12 \
  --threshold 0.80
```

## 运行验证

| 验证项 | 命令或方法 | 预期现象 |
| --- | --- | --- |
| 录音 | `arecord ... sample.wav` | 生成 16 kHz 单声道 WAV。 |
| 离线识别 | `./kws_infer --wav sample_open.wav` | 输出类别和置信度。 |
| 实时识别 | `./kws_light ...` | 终端显示关键词触发日志。 |
| GPIO 联动 | 说出关键词或播放 WAV | LED/继电器状态变化，或日志模拟状态变化。 |

## 常见问题

| 现象 | 可能原因 | 处理方式 |
| --- | --- | --- |
| 录不到声音 | ALSA 设备号错误 | 用 `arecord -l` 重新确认设备。 |
| 识别不稳定 | 噪声、口音或阈值不合适 | 使用固定 WAV 验证，再调阈值。 |
| TFLM 编译失败 | 算子缺失或 arena 太小 | 打印所需 op，增大 tensor arena。 |

## 本讲成果

- 一份 WAV 录音和离线识别结果。
- 一个 KWS 推理程序或脚本。
- 一份 GPIO 控灯或模拟控灯的触发日志。
