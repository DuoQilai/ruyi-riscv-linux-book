# RISC-V AI 推理开发板选型报告（2025-2026）

> 调研时间：2026 年 7 月 | 一个月内采购 | 聚焦 AI 推理性能

---

## 结论：两块板子

| 预算 | 买什么 | 芯片 | AI 引擎 | 推荐模型 | 实测速度 | 一句话 |
|------|--------|------|--------|---------|---------|--------|
| **¥5,000** | **RK1828 M.2 算力卡 + RK3588 主机** | 瑞芯微 RK1828（2025 量产） | 20 TOPS NPU + 5GB 堆叠 DRAM | DeepSeek-R1-Distill-Qwen-7B（INT4） | **52 tok/s** | 买速度——NPU 硬加速，7B 模型聊得动 |
| **¥10,000** | **Milk-V Pioneer** | 算能 SG2042（64 核 C920） | 64 核 CPU + 128GB ECC 内存 | DeepSeek-R1-Distill-Qwen-14B（INT4） | **2.3 tok/s** | 买容量——唯一能装 14B-70B 模型的 RISC-V 机器 |

**为什么 ¥5,000 比 ¥10,000 快 8 倍？** 不是逻辑错了。RK1828 是专用 NPU 加速卡（只有 5GB 内存，跑不动大模型），Pioneer 是 64 核通用 CPU（128GB 内存，什么模型都装得下）。2026 年 RISC-V 市场上没有一个既快又能跑大模型的单品。¥5,000 买推理速度，¥10,000 买模型容量。

**两个都买？** ¥15,000。RK1828 日常跑 AI 推理，Pioneer 做大模型探索 + 64 核编译 + RISC-V 多核优化。覆盖所有场景。

---

## 两块板子的完整信息

### ¥5,000：RK1828 M.2 算力卡 + RK3588 主机

| 项目 | 详情 |
|------|------|
| **芯片** | 瑞芯微 RK1828，3×RISC-V 控制核 + 20 TOPS NPU（INT8）+ 5GB 3D 堆叠 DRAM（~1TB/s 带宽），2025 年量产 |
| **形式** | M.2 2280 Key-M 卡，插在 RK3588 主机上 |
| **主机** | RK3588（8GB+ RAM，ARM SoC），提供 USB/网口/显示输出 |
| **功耗** | 典型 10W，最大 15W，被动散热 |
| **系统** | Linux（RKNN3 SDK） |
| **购买** | 飞凌嵌入式（forlinx.com）、Firefly（天启），M.2 单卡 + 主机 ≈ ¥3,000-5,000 |
| **到手** | 企业渠道，1-2 周 |

**实测 AI 性能：**

| 模型 | 量化 | 速度 |
|------|------|------|
| Qwen2.5-7B | INT4 | **52 tok/s** |
| Qwen2.5-3B | INT4 | **81 tok/s** |
| Llama 2-7B | INT4 | **60-80 tok/s** |
| DeepSeek-R1-Distill-Qwen-7B | INT4 | ~50-80 tok/s（预估，同参数级） |

**限制：** 5GB 内存硬顶，只能跑 ≤7B 模型。不支持 CNN 传统视觉（YOLO/ResNet，不如 RK3588 自带 NPU）。只做 Transformer 推理。

**推荐模型：DeepSeek-R1-Distill-Qwen-7B（INT4，约 3.8GB）**
带推理链（Chain-of-Thought），数学/逻辑/代码比同参数基础模型强一档。中文用 Qwen2.5-7B-Instruct，代码用 DeepSeek-Coder-6.7B。

---

### ¥10,000：Milk-V Pioneer

| 项目 | 详情 |
|------|------|
| **芯片** | 算能 SG2042，64 核 C920 @2.0GHz，RVV 0.7.1，2023 年量产 |
| **内存** | 最高 128GB DDR4 ECC（8 个 DIMM 插槽） |
| **存储** | NVMe SSD |
| **扩展** | PCIe 3.0 x16，ATX 标准主板（塞进任何 PC 机箱） |
| **功耗** | 待机 ~40W，满载 ~120W |
| **系统** | Ubuntu、Debian、Fedora（主线内核） |
| **购买** | Crowd Supply / Milk-V 官方，$1,500 ≈ ¥10,000+（含 128GB 内存） |
| **到手** | 国际快递，2-4 周 |

**实测 AI 性能：**

| 模型 | 量化 | 速度 |
|------|------|------|
| Llama 7B | Q4 | **6.63 tok/s** |
| DeepSeek R1 Distill Llama 8B | Q4 | **4.32 tok/s** |
| DeepSeek R1 Distill Qwen 14B | Q4 | **2.29 tok/s** |
| Llama 70B | Q4 | ~0.5-1 tok/s（能加载，极慢） |

**推荐模型：DeepSeek-R1-Distill-Qwen-14B（INT4，约 7.5GB）**
14B + 推理链，多步推理、代码审查、长文摘要可用。2-3 tok/s 不是聊天速度，是批处理速度——发一句，等几秒开始出字。

**Pioneer 的真正价值：** 128GB ECC 内存能塞 70B 模型做评估和优化实验。64 核能并行编译内核、跑 CI、搭数据库。它是 RISC-V 开发工作站，AI 推理只是其中一个用途。

---

## 前置知识：一块开发板包含什么

```
一块 RISC-V 开发板 =  SoC（CPU/NPU）     ← 芯片，焊死在板上
                    + RAM（内存）         ← 焊死或 DIMM 插槽，模型权重放这里
                    + 存储（eMMC/SSD）    ← 系统和模型文件存这里
                    + IO（网口/USB/PCIe）  ← 外设接口
                    + 电源电路
```

推理流程：硬盘里的模型文件 → 加载到内存 → CPU/NPU 从内存读权重做计算。

两个约束：**NPU/CPU 算力**决定推理速度，**内存大小**决定能装多大的模型。

---

## 买 RISC-V 跑 AI 到底能干什么

把 Qwen 塞进板子对话确实无聊。RISC-V 在 AI 上的真正价值在三个 NVIDIA 做不到的方向：

### 1. 自定义指令加速你的模型

RISC-V 是开源 ISA，可以给 CPU 加自定义指令。中科院团队在香山南湖核上的实验：加了条矢量点积指令，硬件面积只增 2.8%，功耗只增 0.5%，GPT-2 推理快 30%。[《计算机科学》2025 年第 5 期]

如果你有公司内部的特定模型，可以针对它在 RISC-V 上定制指令。x86/ARM 做不到——ISA 不让你改。

### 2. "通推一体"——CPU 和 AI 算力共享内存

传统：CPU ←PCIe→ GPU/NPU，数据搬来搬去，延迟高。

**XSAI（香山 AI）**：北京开源芯片研究院（中科院 + 阿里/腾讯/中兴/算能等 18 家）基于第三代香山昆明湖核，2026 年 4 月开源了全球首个 RISC-V 通推一体处理器。[GitHub: OpenXiangShan/XSAI]

| 能力 | 做了什么 |
|------|--------|
| Matrix 引擎 | bf16/fp8/int8 矩阵乘加，直接接 L2 缓存（不经 PCIe） |
| Vector 引擎 | 硬件 exp2 加速 softmax，与 Matrix 异步并行 |
| FPGA 实测 | Llama-2 15M，Prefill 343 tok/s，Decode 36 tok/s @50MHz |
| 顶会背书 | ISCA 2026 Tutorial |

场景：机器人同时做视觉检测 + 运动控制 + 异常告警。传统方案要多芯片，香山一个芯片全搞定，延迟从毫秒压到微秒。

### 3. 已经在用的 RISC-V AI 集群

南湖核（香山第二代）出货上万片，某国产 GPU 厂集成进智算加速卡，搭了千卡千亿模型算力集群，正往万卡扩。[EET China 2025.8]

进迭时空基于昆明湖研发了 X200 处理器核（50 SPECint2006/Core），AI CPU 芯片 2026 年底量产，目标超级 AI 计算机 + 高阶自动驾驶。

### 工业界落地场景

| 场景 | 谁在做 | 为什么用 RISC-V |
|------|--------|----------------|
| 工业机器人 VLA | MIPS S8200（RISC-V NPU，2026） | 视觉-语言-动作多模态，全本地，<1ms |
| 产线质检 | MIPS S8200 + EclipseX1 | 端侧 Transformer，数据不出工厂 |
| 自动驾驶感知 | MIPS S8200（ForwardEdge ASIC 已选用） | 前摄+360°融合+决策，同一芯片 |
| EV 充电桩调度 | EclipseX1（12 TOPS @10W） | 预测性电力分配，-40°C~85°C |

### 你的团队买板子后干什么

1. **RK1828**：学会 llama.cpp 部署和量化管线，搞清楚 NPU 怎么跑 Transformer。
2. **Pioneer**：研究 64 核 NUMA 并行推理，验证 70B 模型 CPU-only 可行性。
3. **长远**：RISC-V 定制指令 + 通推一体，把公司边缘 AI 产品从 Jetson 切到国产 RISC-V。

---

## 2026 年 RISC-V 笔记本

### DC-ROMA RISC-V Mainboard III（唯一可购）

深度数智（DeepComputing，香港）2026 年 5 月发布，全球首款 RVA23 RISC-V 笔记本主板，适配 Framework Laptop 13。

| 项目 | 详情 |
|------|------|
| **芯片** | 进迭时空 SpacemiT K3（8 核 RVA23 @2.5GHz + 8 核 AI 引擎，60 TOPS） |
| **内存** | 16GB / 32GB LPDDR5 |
| **系统** | Ubuntu 26.04 主线内核 |
| **价格** | $699（16GB，约 ¥5,000）起；完整笔记本 Pro 套装 $1,499（约 ¥11,000）起 |
| **购买** | store.deepcomputing.io，预售，首批 6 月底发货 |
| **国内到手** | Framework 机身需从美国海淘（$849-1,099）+ 香港发货主板 → 总花费 ¥12,000-15,000，约 2 个月 |

**一个月内要拿到手不现实**，但值得放进采购计划。K3 有 60 TOPS NPU，本地跑 7B 模型。进迭时空 K3 的商业化路线是"昆明湖 → X200 → K3"，这块板子本质上是香山第三代生态的跳板。

### 桌面替代：Milk-V Jupiter v1

淘宝现货，进迭时空 K1，Mini-ITX，¥500-1,100。配显示器键盘就是 Linux 桌面。比等笔记本务实。

---

## 香山（XiangShan）生态

中国最强的开源 RISC-V 处理器。北京开源芯片研究院主导，中科院计算所 + 18 家龙头企业联合。

### 三代演进

| 代次 | 代号 | 时间 | 工艺 | SPECint2006 | 对标 ARM | 状态 |
|------|------|------|------|------------|---------|------|
| 第一代 | 雁栖湖 | 2021 | 28nm | 7 分/GHz | A73 | 开源 |
| 第二代 | 南湖 | 2023 | 14nm/2GHz | 10 分/GHz | **A76** | 出货上万片 |
| 第三代 | 昆明湖 | 2025.7 交付 | **7nm/3GHz** | **16.5 分/GHz** | **Neoverse N2** | 首批商业交付 |

### 为什么重要

1. **完全开源。** RTL 代码在 GitHub（OpenXiangShan），每一行 CPU 设计可见。
2. **XSAI 通推一体。** 2026.4 开源，Matrix + Vector 双引擎，FPGA 跑通 Llama-2，ISCA 2026 顶会 Tutorial。
3. **千卡千亿集群在跑。** 南湖核上万片出货，不是 PPT。
4. **进迭时空 X200。** 基于昆明湖，50 SPECint2006/Core，2026 年底量产。

### 香山 + Linux

| 发行版 | 状态 |
|--------|------|
| **openRuyi（如意）** | 2026.3 发布，原生适配香山，开机快 40%，功耗降 30% |
| **Ubuntu 26.04 LTS** | 2026.4 发布，首次原生支持 RISC-V |
| Debian、Fedora、openEuler | 均已适配 |

### 香山和采购的关系

今天买不到香山硬件。但进迭时空 K3 是昆明湖商业化的产物。现在买 K3 板子（Jupiter、DC-ROMA III），软件栈和优化经验将来直接迁移到纯血香山平台。

---

## 其他档位速览

### ¥1,000 档

| 型号 | 芯片 | 价格 | 推荐模型 | 速度 |
|------|------|------|---------|------|
| **Banana Pi BPI-F3** | 进迭时空 K1，8 核 + RVV 1.0 | **¥700-950** | TinyLlama 1.1B Q4 | ~5-8 tok/s |
| Milk-V Jupiter v1 | 进迭时空 K1/K1X | ¥500-800 | 同上 | 同上 |
| MUSE PI | 进迭时空 K1 + 2 TOPS NPU | ¥380-500 | 同上 | 同上 |

¥1,000 档无 NPU，纯 CPU 软跑，7B 只有 1-3 tok/s。价值在学 llama.cpp 和 RVV 编程。

### ¥50,000 档

| 方案 | 芯片 | 价格 |
|------|------|------|
| 算能 SG2044 开发平台（联系算能） | SG2044，64 核 + RVV 1.0 | 需询价，预估 ¥30,000-50,000 |
| 3× Milk-V Pioneer 集群 | 192 核，384GB 总内存 | ¥30,000-40,000 |
| 等玄铁 C950 硬件（2026 Q3-Q4） | C950，5nm，DeepSeek V3 671B @18 tok/s | 预估 ¥50,000+ |

只跑 AI 推理，¥50,000 买 NVIDIA 强 10-100 倍。买 RISC-V 服务器只有两个理由：信创合规、RISC-V 软件栈研发。

---

## 参考来源

- 瑞芯微 RK1828 实测：[iotdt.com](http://mp.weixin.qq.com/s?__biz=MzkwODQxNjQzMg==&mid=2247485638&idx=1&sn=feb09cf02712f475e45d1d509d8c25eb)
- 阿里玄铁 C950：[cls.cn](https://api3.cls.cn/share/article/2323452)
- SG2042 llama.cpp：[RISC-V Summit Europe 2025](https://riscv-europe.org/summit/2025/media/proceedings/)
- SG2044 HPC 评估：[arXiv 2508.13840](https://arxiv.org/abs/2508.13840)
- Milk-V Pioneer：[Crowd Supply](https://www.crowdsupply.com/milk-v/milk-v-pioneer)
- Milk-V Titan：[ruyisdk.cn](https://ruyisdk.cn/t/topic/2405)
- 进迭时空 K3：[pedaily.cn](https://news.pedaily.cn/202601/560019.shtml)
- 进迭时空 B 轮：[laoyaoba.com](https://www.laoyaoba.com/html/share/news/974423)
- DC-ROMA III：[TechPowerUp](https://www.techpowerup.com/349005/) / [IT之家](https://m.ithome.com/html/949916.htm)
- 香山第三代商业化：[快科技](http://m.mydrivers.com/newsview/1066704.html) / [EET China](https://www.eet-china.com/info/73756.html)
- 如意香山本：[IT之家](https://digi.ithome.com/archiver/791/063.htm)
- openRuyi + 香山：[中新网](https://www.chinanews.com.cn/gn/2026/03-27/10593800.shtml)
- RISC-V 2026 开发板指南：[Luca Berton](https://lucaberton.com/blog/risc-v-development-boards-2026-guide/)
- RISC-V 数据中心：[Luca Berton](https://lucaberton.com/blog/risc-v-datacenter-servers-sovereign-ai-2026/)
- Andes RVV 实测：[RISC-V Summit Europe 2025](https://riscv-europe.org/summit/2025/media/proceedings/2025-05-13-RISC-V-Summit-Europe-P1.1.08-LEE-poster.pdf)
