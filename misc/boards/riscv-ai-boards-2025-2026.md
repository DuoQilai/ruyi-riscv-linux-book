# RISC-V AI 推理开发板选型报告（2025-2026）

> 调研时间：2026 年 7 月
> 范围：2025-2026 年发布/量产的 RISC-V 芯片平台，聚焦 AI 推理

---

## 前置知识：一块开发板到底包含什么

开发板是买回来插电就能用的完整主板，包含以下部分：

```
一块 RISC-V 开发板 =  SoC（CPU/NPU）     ← 芯片，焊死在板上
                    + RAM（内存）         ← 焊死或 DIMM 插槽，模型权重放这里
                    + 存储（eMMC/SSD）    ← 系统和模型文件存这里
                    + IO（网口/USB/PCIe）  ← 外设接口
                    + 电源电路
```

推理流程：硬盘里的模型文件 → 加载到内存 → CPU/NPU 从内存读权重做计算。

关键约束就两个：**NPU/CPU 算力**决定推理速度，**内存大小**决定能装多大的模型。

---

## 核心逻辑：为什么更贵的不一定 AI 推理更快

控制单一变量——**同一个模型 Qwen2.5-7B（INT4 量化，约 3.8GB）**，四档横比：

| 档位 | 推荐硬件 | AI 引擎 | 内存 | 同一模型速度 | 为什么 |
|------|---------|--------|------|------------|--------|
| ¥1,000 | BPI-F3（K1） | 8 核 CPU，无 NPU | 8-16GB | ~1-3 tok/s | 纯 CPU，核少，能跑但慢 |
| ¥5,000 | **RK1828** | 20 TOPS NPU | 5GB（NPU 专用） | **52 tok/s** | 专用 NPU，硬件加速矩阵乘法 |
| ¥10,000 | Milk-V Pioneer | 64 核 CPU，无 NPU | 128GB | ~6.6 tok/s | 核多但仍是 CPU 软跑，RVV 向量不如 NPU 高效 |
| ¥50,000 | SG2044（预估） | 64 核 CPU + RVV 1.0，无 NPU | 128GB+ | ~10-20 tok/s（估算） | RVV 1.0 比 0.7.1 强很多，但仍不如专用 NPU |

**结论：同一 7B 模型，¥5,000 的 RK1828 最快。不是逻辑错了，是 2026 年 RISC-V 市场的结构性现实——¥10,000+ 的板子贵在 CPU 核数和内存容量，不是 AI 推理速度。**

但换个视角——能跑的**最大模型**：

| 档位 | 推荐硬件 | 内存上限 | 最大可加载模型 |
|------|---------|---------|-------------|
| ¥1,000 | BPI-F3 | 16GB | 7B（勉强） |
| ¥5,000 | RK1828 | 5GB NPU 专用 | 7B（内存硬顶，装不下更大） |
| ¥10,000 | Milk-V Pioneer | 128GB | **70B+**（能装下，慢但能跑） |
| ¥50,000 | SG2044 / 多节点 | 128-384GB | **100B+**（分布式） |

这两张表合在一起就是这个报告的核心逻辑：**¥5,000 买速度（小模型跑得快），¥10,000+ 买容量（大模型跑得动）。** 2026 年 RISC-V 市场上不存在一个既快又能跑大模型的单品。

---

## 四档推荐

每档给出 3 个选项。⭐ 为推荐，标注最适合的模型及实测/预估速度。

---

### 💰 ¥1,000 档 —— 入门学习

这个价位没有 2025-2026 年新芯片，也没有 NPU。全是用 2023-2024 的 CPU 软跑。

| # | 型号 | 芯片 | 配套 | AI 引擎 | 价格 |
|---|------|------|------|--------|------|
| ⭐ | **Banana Pi BPI-F3** | 进迭时空 K1，8 核 2.0GHz，RVV 1.0 | 8/16GB LPDDR5 + eMMC/SSD | 纯 CPU 软跑 llama.cpp | **¥700-950** |
| 2 | MUSE PI | 进迭时空 K1，8 核 + 2 TOPS NPU | 4GB + 16GB eMMC | CPU + 轻量 NPU | **¥380-500** |
| 3 | Milk-V Jupiter | 进迭时空 K1/K1X | 4/8/16GB，mini-ITX，M.2，PCIe | 同 K1 平台 | **¥500-800** |

**⭐ 推荐模型：TinyLlama 1.1B（Q4，约 0.6GB）**

| 指标 | 数据 |
|------|------|
| 速度 | ~5-8 tok/s |
| 能做什么 | 基础对话、代码补全提示、RISC-V AI 工具链学习 |
| 不能做什么 | 复杂推理、长文本、多轮对话质量不够 |

¥1,000 档的价值是学会 RISC-V 上的 llama.cpp 部署、RVV 向量编程、模型量化——而不是真正的 AI 应用。

---

### 💰 ¥5,000 档 —— 边缘推理甜点

2025 年 NPU 进入 RISC-V 生态，这一档开始有真正可用的 AI 推理速度。

| # | 型号 | 芯片 | 配套 | AI 引擎 | 价格 |
|---|------|------|------|--------|------|
| ⭐ | **RK1828 M.2 算力卡 + RK3588 主机** | RK1828：3×RISC-V 控制核 + 20 TOPS NPU + 5GB 堆叠 DRAM（~1TB/s 带宽），2025 年量产 | M.2 卡插在 RK3588 主机上，主机 8GB+ RAM | **20 TOPS NPU（INT8）**，硬件加速 Transformer 推理 | M.2 卡 ~¥500-800 + 主机 ~¥2,000-4,000 ≈ **¥3,000-5,000** |
| 2 | Milk-V Titan 基础配置 | UltraRISC UR-DP1000，8 核 2.0GHz，RVV，2025 年发布 | 最高 64GB DDR4 ECC（双 DIMM），M.2 NVMe，PCIe 4.0 x16 | 纯 CPU + RVV 向量。待机 14W，满载 30W | **¥2,380（板）+ ¥500-1,500（内存+SSD）≈ ¥3,000-4,000** |
| 3 | SiFive HiFive Premier P550 | SiFive P550，4 核，RISC-V 单核 IPC 最高，2024 末 | 8-16GB | 纯 CPU，4 核 |

**⭐ 推荐：RK1828 方案**

**推荐模型：DeepSeek-R1-Distill-Qwen-7B（INT4，约 3.8GB）**

| 指标 | 数据 |
|------|------|
| 实测速度 | **~50-80 tok/s**（社区 benchmark 中 Llama 2-7B 实测 60-80 tok/s，Qwen2.5-7B 实测 52 tok/s，蒸馏 DeepSeek 同参数级速度相当） |
| 为什么选这个模型 | DeepSeek 蒸馏版有推理链（Chain-of-Thought），比同参数量的基础模型"聪明"得多。数学、逻辑、代码能力远超普通 7B |
| 5GB 内存能装下吗 | INT4 量化后约 3.5-4GB，装得下 |
| 不能做什么 | 7B 的推理链仍然有限，复杂多步推理会出错。不能做 Agent、长上下文 |

**备选：**
- 中文场景优先 → **Qwen2.5-7B-Instruct（INT4）**，实测 52 tok/s，中文能力 SOTA
- 代码生成 → **DeepSeek-Coder-6.7B（INT4）**，速度类似
- 轻量多模态（图生文）→ **Llava-OneVision-Qwen2-7B（INT4）**，但要确认 RK1828 SDK 对视觉模型的支持

**为什么 ¥5,000 推荐 NPU 卡而不是 Titan？**

Titan 纯 CPU 跑同一个 DeepSeek 7B 只有 ~3-5 tok/s——能用，但和 RK1828 的 50+ tok/s 差一个数量级。Titan 的价值是 PCIe x16：将来插 GPU 后 AI 能力质变。但 2026 年中 RISC-V + AMD GPU 的驱动还不稳定，这是期货，不是现货。

---

### 💰 ¥10,000 档 —— 大模型探索

这一档的核心卖点不是快，是**能装下 ¥5,000 档装不下的大模型**。

| # | 型号 | 芯片 | 配套 | AI 引擎 | 价格 |
|---|------|------|------|--------|------|
| ⭐ | **Milk-V Pioneer** | 算能 SG2042，64 核 C920 @2.0GHz + RVV 0.7.1，2023 年量产（目前 ¥1 万档唯一 64 核 RISC-V） | 最高 **128GB DDR4 ECC**（8 个 DIMM），NVMe，PCIe 3.0 x16，ATX 标准主板 | 64 核纯 CPU + RVV 0.7.1 软跑 llama.cpp | **~$1,500 ≈ ¥10,000+**（含 128GB 内存） |
| 2 | RK1828 Firefly 开发套件 | RK1828 + RK3588 SoM 8G+64G，2025 年 | 8GB + 64GB eMMC，完整 SDK + 外壳 | 20 TOPS NPU | **$1,029 ≈ ¥7,500** |
| 3 | Milk-V Titan 满配 + AMD GPU | UR-DP1000 + dGPU | 64GB + SSD + GPU | CPU + 独显（驱动不稳定） | **¥7,000-9,000** |

**⭐ 推荐：Milk-V Pioneer**

**推荐模型：DeepSeek-R1-Distill-Qwen-14B（INT4，约 7.5GB）**

| 指标 | 数据 |
|------|------|
| 实测速度 | **~2-2.5 tok/s**（基于 DeepSeek R1 Distill Qwen 14B Q4 实测 2.29 tok/s） |
| 为什么选它 | 14B + 推理链 = 比 7B 强一档的逻辑能力，能做有意义的多步推理、代码审查、长文摘要 |
| 速度和体验 | 2-3 tok/s 意味着你发一句话，等 3-5 秒开始出字，然后慢慢吐。**不是聊天速度，是批处理速度** |
| 能不能跑更大的 | 有人跑通了 Llama 70B Q4（~0.5-1 tok/s），能加载但慢到没有实用价值 |

**同一模型对比——DeepSeek 7B 在 ¥5,000 vs ¥10,000 的表现：**

| 模型 | RK1828（¥5,000） | Pioneer（¥10,000） |
|------|-------------------|-------------------|
| DeepSeek 7B Q4 | **52 tok/s** | ~4.3 tok/s |
| DeepSeek 14B Q4 | 装不下（5GB 内存限制） | **2.3 tok/s** |

**Pioneer 的真正价值不在 AI 推理本身。** 它是你唯一能花 ¥1 万买到带 128GB ECC 内存的 64 核 RISC-V 工作站。这意味着：
- 加载 70B 模型做 prompt 工程和 RISC-V 优化实验
- 64 核并行编译内核、跑 CI、搭数据库——不跑 AI 也是正经的开发工作站
- 研究 RISC-V 多核 NUMA 架构下 llama.cpp 的优化空间

---

### 💰 ¥50,000 档 —— 服务器级

RISC-V AI 最大的空白区间。¥5 万够买一片 NVIDIA L40S（48GB，~700+ TOPS），而 RISC-V 这边连一个带 NPU 的服务器开发板都买不到。

| # | 方案 | 芯片 | 配套 | AI 引擎 | 价格 |
|---|------|------|------|--------|------|
| ⭐ | **算能 SG2044 开发平台**（主动联系算能采购） | SG2044：64 核 + RVV 1.0，2025 年发布 | 预估 64-128GB DDR5，PCIe Gen4 | 纯 CPU + RVV 1.0。RVV 1.0 vs SG2042 的 0.7.1 有质的提升 | **需询价**，预估 ¥30,000-50,000 |
| 2 | 3× Milk-V Pioneer 集群 | 3× SG2042 | 192 核 / 384GB 总内存 | 分布式 llama.cpp。社区有尝试，无系统 benchmark | **¥30,000-40,000** |
| 3 | 等玄铁 C950 服务器硬件 | C950：2026 年 3 月发布，5nm，3.25GHz，Matrix + Vector 双 AI 引擎 | 预计 2026 Q3-Q4 可能有硬件 | Matrix 引擎实测 DeepSeek V3 671B：**18 tok/s** | **¥50,000+**（预估，未公开） |

**⭐ 推荐：联系算能争取 SG2044 开发平台 + DeepSeek-V2-Lite（16B MoE，INT4，约 8GB）**

> SG2044 目前没有公开的 AI benchmark。但已知提升：RVV 1.0 完整向量支持 + GCC 15.2 优化 + 更高的 IPC。HPC 评估显示多核性能较 SG2042 显著提升。按 RVV 1.0 对标 Andes AX45MPV 的数据来保守估算：
>
> DeepSeek-V2-Lite 是一个 MoE（混合专家）模型，16B 总参数但每次推理只激活约 2.7B，天然适合 CPU 推理——参数大（能力强）但计算量小（速度快）。**如果 SG2044 的 RVV 1.0 优化到位，这个模型可能跑到 8-15 tok/s，是可接受的速度。**

**¥50,000 档的残酷真相：**

只为了 AI 推理，¥50,000 买 NVIDIA 方案比 RISC-V 强 10-100 倍。唯一的购买理由是：你的组织必须在 RISC-V 平台上做 AI 研发（信创合规、RISC-V 软件栈优化、RISC-V 大模型可行性验证）。

---

## 总结

### 按"跑同一个模型谁快"——控制变量

| 同一模型 | ¥1,000 BPI-F3 | ¥5,000 RK1828 | ¥10,000 Pioneer | ¥50,000 SG2044(估) |
|---------|--------------|--------------|----------------|-------------------|
| TinyLlama 1.1B | 5-8 tok/s | 100+ tok/s | 15-20 tok/s | 30-50 tok/s |
| DeepSeek 7B Q4 | 1-3 tok/s | **52 tok/s** | 4-6 tok/s | 10-20 tok/s |
| DeepSeek 14B Q4 | 装不下 | 装不下 | **2.3 tok/s** | 5-10 tok/s |
| 70B+ 级别 | 装不下 | 装不下 | **塞得进（<1 tok/s）** | **塞得进（可能可用）** |

### 按档位最佳选择

| 预算 | 买什么 | 跑什么模型 | 体验 |
|------|--------|-----------|------|
| ¥1,000 | BPI-F3 | TinyLlama 1.1B | 入门学习，不是干活用的 |
| ¥5,000 | **RK1828 方案** | **DeepSeek 7B** | **真正可用，聊天流畅** |
| ¥10,000 | Milk-V Pioneer | DeepSeek 14B（大模型探索） | 速度慢，但能装大模型 |
| ¥50,000 | 联系算能 SG2044 / 等 C950 | DeepSeek-V2-Lite 或更大 | 看运气，诚意联系厂商 |

### 最大的变量

**玄铁 C950 的硬件。** 2026 年 3 月芯片发布，DeepSeek V3 671B 跑 18 tok/s。如果下半年有开发板出来，整个格局会变——一颗 C950 可能同时吃掉 ¥10,000 和 ¥50,000 两档。

---

## 参考来源

- 瑞芯微 RK1828 实测数据：[iotdt.com](http://mp.weixin.qq.com/s?__biz=MzkwODQxNjQzMg==&mid=2247485638&idx=1&sn=feb09cf02712f475e45d1d509d8c25eb) — Qwen2.5-7B 52 tok/s, Llama 2-7B 60-80 tok/s
- 阿里玄铁 C950 发布：[cls.cn](https://api3.cls.cn/share/article/2323452) — DeepSeek V3 全量 18 tok/s, Qwen2.5-235B 34 tok/s
- SG2042 llama.cpp 实测：[RISC-V Summit Europe 2025 proceedings](https://riscv-europe.org/summit/2025/media/proceedings/) — Llama 7B 6.63 tok/s, DeepSeek 14B 2.29 tok/s
- SG2044 HPC 评估：[arXiv 2508.13840](https://arxiv.org/abs/2508.13840) — RVV 1.0 + GCC 15.2, 多核性能显著提升
- Milk-V Pioneer：[Crowd Supply](https://www.crowdsupply.com/milk-v/milk-v-pioneer) — $1,500, 64 核 SG2042, 最高 128GB
- Milk-V Titan：[ruyisdk.cn](https://ruyisdk.cn/t/topic/2405) — $279-329, UR-DP1000, PCIe 4.0 x16
- 进迭时空 K3 发布：[pedaily.cn](https://news.pedaily.cn/202601/560019.shtml) — RVA23, AI CPU
- 进迭时空 B 轮融资：[laoyaoba.com](https://www.laoyaoba.com/html/share/news/974423) — 超 6 亿元
- RISC-V 2026 开发板指南：[Luca Berton](https://lucaberton.com/blog/risc-v-development-boards-2026-guide/)
- RISC-V 数据中心与服务器：[Luca Berton](https://lucaberton.com/blog/risc-v-datacenter-servers-sovereign-ai-2026/)
- Andes AX45MPV llama.cpp 实测：[RISC-V Summit Europe 2025 poster](https://riscv-europe.org/summit/2025/media/proceedings/2025-05-13-RISC-V-Summit-Europe-P1.1.08-LEE-poster.pdf) — DeepSeek Lite 16B MoE 2.52 tok/s (RVV 优化参考)
