# 6.4 大语言模型与视觉大语言模型

## 对应大纲

大纲讲次原文：第4讲 大语言模型与视觉大语言模型。
大纲知识点原文：LLM 推理基础；llama.cpp RISC-V 编译；小型 LLM 本地部署；LLM + 传感器融合；视觉大语言模型（VLM）概述；VLM RISC-V 部署与调优。

LLM 推理基础、llama.cpp RISC-V 编译、小型 LLM 本地部署、LLM + 传感器融合、视觉大语言模型（VLM）概述、VLM RISC-V 部署与调优。

## 目标

学生能理解小型 LLM/VLM 在边缘板卡上的部署限制，使用 llama.cpp 完成一次文本问答；条件允许时完成图片问答或传感器数据问答演示。

## 知识点

| # | 知识点 | 内容 |
| --- | --- | --- |
| 1 | LLM 推理基础 | Transformer、自回归解码、KV Cache 和 tokenizer。 |
| 2 | llama.cpp RISC-V 编译 | CMake、GGML、量化格式和 RISC-V/RVV 编译选项。 |
| 3 | 小型 LLM 本地部署 | Qwen/SmolLM 等轻量模型、prompt 模板和流式输出。 |
| 4 | LLM + 传感器融合 | 将 JSON 传感器数据放入 prompt，生成解释或控制建议。 |
| 5 | VLM 概述 | vision encoder、projection、LLM 和多模态输入。 |
| 6 | VLM RISC-V 部署与调优 | llama.cpp llava 路径、图片预处理、内存和延迟取舍。 |

## 讲授要点

- LicheePi 4A 上的小模型演示重点是“能部署、能测量、能解释限制”，不是追求大模型能力。
- 量化格式会影响模型大小、速度和质量，报告必须写明 GGUF 文件名和量化类型。
- KV Cache 会随上下文长度增长，占用内存；长 prompt 可能导致明显变慢或 OOM。
- VLM 比文本 LLM 多出图像编码器，硬件和内存条件不足时可改做静态图片预处理和 PC 侧对照。
- 传感器融合示例可以用第 2/4 章已有 JSON 数据，不需要重新搭建复杂系统。

## 操作或演示

编译或使用预编译 llama.cpp：

```bash
cd ~/ai-labs/llm
cmake -S llama.cpp -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j2
```

文本问答：

```bash
./build/bin/llama-cli \
  -m models/small-model.Q4_K_M.gguf \
  -p "请用三句话解释 RISC-V RVV 的优势。" \
  -n 128
```

传感器融合 prompt：

```bash
./build/bin/llama-cli \
  -m models/small-model.Q4_K_M.gguf \
  -p "传感器数据：{\"temp\":31.2,\"humidity\":68,\"fan\":\"off\"}。请判断是否需要打开风扇，只回答建议和理由。" \
  -n 96
```

VLM 条件验证：

```bash
./build/bin/llava-cli \
  -m models/vlm-text.Q4_K_M.gguf \
  --mmproj models/mmproj.gguf \
  --image test.jpg \
  -p "图中主要物体是什么？" \
  -n 64
```

## 运行验证

| 验证项 | 命令或方法 | 预期现象 |
| --- | --- | --- |
| 二进制验证 | `./build/bin/llama-cli --help` | 能输出帮助信息。 |
| 文本问答 | 运行小模型 prompt | 能流式输出文本。 |
| 资源记录 | `time`、`free -h`、程序 token/s | 记录耗时、内存和 token 速度。 |
| VLM 替代验证 | `llava-cli` 或记录无法运行原因 | 有图像问答输出，或明确缺少模型/内存。 |

## 常见问题

| 现象 | 可能原因 | 处理方式 |
| --- | --- | --- |
| 模型无法加载 | 内存不足或文件损坏 | 换更小量化模型，检查 SHA256。 |
| 输出很慢 | 模型过大或上下文过长 | 缩短 prompt，降低 `-n`，使用更小模型。 |
| VLM 运行失败 | 缺少 mmproj 或内存不足 | 先完成文本 LLM，VLM 标记为待补测。 |

## 本讲成果

- 一次小型 LLM 文本问答记录。
- 一份模型文件、量化格式、内存和 token 速度记录。
- 一个传感器 JSON 问答或 VLM 图片问答演示，条件不足时提交替代验证说明。
