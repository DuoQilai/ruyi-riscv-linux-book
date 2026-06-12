# 实验 6.3：语音关键词唤醒控灯

## 对应讲次

| 项目 | 内容 |
| --- | --- |
| 章节 | 第 6 章 机器学习 |
| 讲次 | 第 3 讲 TinyML 边缘推理实战 |
| 课程主题 | USB 麦克风、MFCC、int8 KWS、GPIO 控灯 |
| 实验类型 | 必做实验 |

大纲讲次原文：第3讲 TinyML 边缘推理实战。
大纲实验原文：USB 麦克风 + LicheePi 4A 实现"开灯""关灯"语音关键词唤醒，GPIO 控制 LED/继电器
大纲知识点原文：TinyML 概念与适用场景；TensorFlow Lite Micro 编译；模型训练与导出；语音特征提取（MFCC）；int8 推理引擎集成；关键词唤醒控灯系统。

## 实验目标

- 使用 ALSA 录制 16 kHz 单声道 WAV。
- 对 WAV 文件运行 KWS int8 模型，输出类别和置信度。
- 条件允许时实时监听麦克风并控制 GPIO；没有外设时输出模拟控灯日志。

## 对应知识点

| # | 知识点 | 本实验中的验证方式 |
| --- | --- | --- |
| 1 | TinyML 概念与适用场景 | 使用小型 KWS 模型做端侧推理。 |
| 2 | TensorFlow Lite Micro 编译 | 运行 TFLM 或等价轻量推理程序。 |
| 3 | 模型训练与导出 | 记录 `.tflite` 或 C array 模型来源。 |
| 4 | 语音特征提取（MFCC） | 保存或打印 MFCC 维度。 |
| 5 | int8 推理引擎集成 | 输出量化模型推理类别。 |
| 6 | 关键词唤醒控灯系统 | 关键词触发 GPIO 或模拟状态变化。 |

## 实验环境

| 项目 | 要求 |
| --- | --- |
| 主机环境 | 可准备 WAV 和模型文件 |
| 目标板 | LicheePi 4A |
| 目标系统 | RevyOS |
| 硬件连接 | USB 麦克风；可选 LED + 限流电阻或继电器模块 |
| 软件依赖 | `alsa-utils`、`gpiod`、KWS 推理程序 |

## 硬件连接或部署关系

| 模块 | 引脚/接口 | 连接说明 | 注意事项 |
| --- | --- | --- | --- |
| USB 麦克风 | USB | 插入板端 USB 口 | 用 `arecord -l` 确认设备 |
| LED | GPIO + GND | 串联限流电阻后接 GPIO | 遵循第 2 章接线安全 |
| 继电器 | GPIO/电源 | 可替代 LED 控制负载 | 负载供电需独立确认 |

## 实验任务

### 任务 1：录音和离线识别

录制“开灯”“关灯”和背景噪声样本，运行离线识别。

### 任务 2：实时识别

使用滑动窗口从麦克风采集音频，输出关键词和置信度。

### 任务 3：控灯联动

当“开灯”置信度超过阈值时置 GPIO 高电平，“关灯”时置低电平；无 GPIO 时打印模拟状态。

## 实验步骤

```bash
sudo apt install -y alsa-utils gpiod
mkdir -p ~/ai-labs/kws/{models,wav,results}
cd ~/ai-labs/kws
arecord -l | tee results/arecord_devices.txt
arecord -D plughw:0,0 -f S16_LE -r 16000 -c 1 -d 2 wav/open.wav
arecord -D plughw:0,0 -f S16_LE -r 16000 -c 1 -d 2 wav/close.wav
```

```bash
./kws_infer --model models/kws_int8.tflite --wav wav/open.wav \
  | tee results/open_infer.txt
./kws_infer --model models/kws_int8.tflite --wav wav/close.wav \
  | tee results/close_infer.txt
```

实时控灯：

```bash
sudo ./kws_light \
  --model models/kws_int8.tflite \
  --device plughw:0,0 \
  --gpiochip gpiochip0 \
  --line 12 \
  --threshold 0.80 | tee results/kws_light.txt
```

替代验证：

```bash
./kws_light --model models/kws_int8.tflite --wav wav/open.wav --simulate-gpio
```

## 运行验证

| 验证项 | 预期现象 | 是否通过 |
| --- | --- | --- |
| 录音 | 生成 16 kHz 单声道 WAV |  |
| 离线推理 | 输出 `open/close/unknown` 和置信度 |  |
| 实时推理 | 终端持续输出识别结果 |  |
| 控灯 | LED/继电器变化或模拟日志变化 |  |

## 验收记录

| 记录项 | 内容 |
| --- | --- |
| 运行命令 | `sudo ./kws_light --model ... --threshold 0.80` |
| 关键输出 | 类别、置信度、触发时间、GPIO 状态 |
| 截图或照片 | 终端日志、LED 状态照片 |
| 异常处理 | 无麦克风或 GPIO 时提交 WAV 离线和模拟 GPIO 结果 |

## 常见问题

| 现象 | 可能原因 | 处理方式 |
| --- | --- | --- |
| 录音为空 | 设备号错误或输入音量低 | 用 `alsamixer` 和 `arecord -l` 检查。 |
| 误触发多 | 阈值过低或噪声大 | 提高阈值，增加背景噪声类别。 |
| GPIO 无反应 | 引脚号或权限错误 | 用 `gpioinfo` 确认 line，使用 `sudo`。 |

## 提交要求

- 实验记录：麦克风设备、采样率、模型、阈值。
- 运行截图：离线推理和实时触发日志。
- 源码或配置文件：KWS 程序、GPIO 配置。
- 简短说明：实测识别情况和误触发条件。
