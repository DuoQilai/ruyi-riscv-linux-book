# 实验 6.4：LLM/VLM 边缘演示

## 对应讲次

| 项目 | 内容 |
| --- | --- |
| 章节 | 第 6 章 机器学习 |
| 讲次 | 第 4 讲 大语言模型与视觉大语言模型 |
| 课程主题 | llama.cpp、小型 LLM、传感器融合、VLM |
| 实验类型 | 拓展实验 / 阶段演示 |

大纲讲次原文：第4讲 大语言模型与视觉大语言模型。
大纲实验原文：LicheePi 4A 部署视觉大语言模型（llama.cpp），完成图像理解与问答演示
大纲知识点原文：LLM 推理基础；llama.cpp RISC-V 编译；小型 LLM 本地部署；LLM + 传感器融合；视觉大语言模型（VLM）概述；VLM RISC-V 部署与调优。

## 实验目标

- 编译或运行 llama.cpp，完成一次小型 LLM 文本问答。
- 记录模型名、量化格式、内存占用、生成速度和耗时。
- 条件允许时运行 VLM 图片问答；条件不足时提交文本 LLM 或传感器 JSON 问答替代验证。

## 对应知识点

| # | 知识点 | 本实验中的验证方式 |
| --- | --- | --- |
| 1 | LLM 推理基础 | 观察 prompt、token 输出和上下文长度影响。 |
| 2 | llama.cpp RISC-V 编译 | 编译或运行 RISC-V 二进制。 |
| 3 | 小型 LLM 本地部署 | 使用 GGUF 量化小模型完成问答。 |
| 4 | LLM + 传感器融合 | 输入 JSON 传感器数据并生成建议。 |
| 5 | VLM 概述 | 使用图片和文本 prompt。 |
| 6 | VLM RISC-V 部署与调优 | 记录 VLM 模型、mmproj、内存和失败原因。 |

## 实验环境

| 项目 | 要求 |
| --- | --- |
| 主机环境 | 可准备 GGUF 模型并传输到板端 |
| 目标板 | LicheePi 4A，建议 8GB RAM |
| 目标系统 | RevyOS |
| 硬件连接 | 可选 USB 摄像头；VLM 也可用静态图片 |
| 软件依赖 | llama.cpp、CMake、C++ 编译器、模型文件 |

## 硬件连接或部署关系

| 模块 | 引脚/接口 | 连接说明 | 注意事项 |
| --- | --- | --- | --- |
| 模型文件 | 文件系统 | 放到 `~/ai-labs/llm/models` | 记录来源和 SHA256 |
| 静态图片 | 文件系统 | 放到 `~/ai-labs/llm/images` | VLM 可用，不要求摄像头 |
| 传感器数据 | JSON 文件 | 可复用前序章节数据 | 无传感器时手写示例 JSON |

## 实验任务

### 任务 1：文本 LLM 问答

运行一个小型量化 GGUF 模型，完成指定 prompt，并记录 token 速度。

### 任务 2：传感器融合问答

把温湿度或风扇状态 JSON 放入 prompt，让模型给出简短建议。

### 任务 3：VLM 图片问答

使用 llama.cpp 的 llava 路径加载文本模型、mmproj 和图片，完成图片问答；如失败，记录资源限制。

## 实验步骤

```bash
mkdir -p ~/ai-labs/llm/{models,images,results}
cd ~/ai-labs/llm
free -h | tee results/memory_before.txt
sha256sum models/*.gguf | tee results/model_sha256.txt
```

文本问答：

```bash
/usr/bin/time -v ./llama.cpp/build/bin/llama-cli \
  -m models/small-model.Q4_K_M.gguf \
  -p "请用三句话说明 LicheePi 4A 适合做哪些边缘智能实验。" \
  -n 128 2>&1 | tee results/text_qa.txt
```

传感器融合：

```bash
/usr/bin/time -v ./llama.cpp/build/bin/llama-cli \
  -m models/small-model.Q4_K_M.gguf \
  -p "传感器数据：{\"temp\":31.2,\"humidity\":68,\"fan\":\"off\"}。请判断是否建议打开风扇，只回答建议和理由。" \
  -n 96 2>&1 | tee results/sensor_qa.txt
```

VLM 条件验证：

```bash
/usr/bin/time -v ./llama.cpp/build/bin/llava-cli \
  -m models/vlm-text.Q4_K_M.gguf \
  --mmproj models/mmproj.gguf \
  --image images/test.jpg \
  -p "图中主要物体是什么？" \
  -n 64 2>&1 | tee results/vlm_qa.txt
```

## 运行验证

| 验证项 | 预期现象 | 是否通过 |
| --- | --- | --- |
| llama.cpp 可运行 | `llama-cli --help` 有输出 |  |
| 文本问答 | 模型能生成中文或英文回答 |  |
| 资源记录 | `time -v` 记录最大内存和耗时 |  |
| VLM 问答 | 成功输出图片理解结果，或记录失败原因 |  |

## 验收记录

| 记录项 | 内容 |
| --- | --- |
| 运行命令 | `llama-cli -m models/small-model.Q4_K_M.gguf ...` |
| 关键输出 | 模型回答、token/s、最大内存、总耗时 |
| 截图或照片 | 终端输出、VLM 输入图片和回答 |
| 异常处理 | OOM、模型缺失或 VLM 不可用时提交文本 LLM 替代验证 |

## 常见问题

| 现象 | 可能原因 | 处理方式 |
| --- | --- | --- |
| 模型加载失败 | 文件损坏或内存不足 | 校验 SHA256，换更小量化模型。 |
| 生成速度很慢 | 模型过大或上下文过长 | 降低 `-n`，换更小模型。 |
| VLM 缺少 mmproj | 模型文件不完整 | 补齐 mmproj 或改做文本问答。 |

## 提交要求

- 实验记录：模型名、量化格式、文件大小、运行参数。
- 运行截图：文本问答、传感器问答、可选 VLM 问答。
- 源码或配置文件：运行脚本、模型清单。
- 简短说明：哪些结果为板端实测，哪些为替代验证或待补测。
