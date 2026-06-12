# 实验 5.4：SGEMM 性能瓶颈分析

## 对应讲次

| 项目 | 内容 |
| --- | --- |
| 章节 | 第 5 章 RISC-V 向量扩展（RVV）加速编程 |
| 讲次 | 第 4 讲 向量化性能分析与调优 |
| 课程主题 | `perf`、统计方法、对齐、展开、瓶颈分析 |
| 实验类型 | 必做实验 |

大纲讲次原文：第4讲 向量化性能分析与调优。
大纲实验原文：使用 perf 分析上一讲 SGEMM 性能瓶颈，逐项优化并记录加速比变化
大纲知识点原文：性能测量方法论；perf 工具使用；内存带宽瓶颈分析；循环展开与软件流水；数据对齐与预取；RVV 与 OpenBLAS 集成。

## 实验目标

- 对第 5.3 讲 SGEMM 做 `perf stat` 和热点分析。
- 实施至少两项优化：如数据对齐、B 矩阵转置、循环展开、分块。
- 形成优化前后对比表和简短分析。

## 对应知识点

| # | 知识点 | 本实验中的验证方式 |
| --- | --- | --- |
| 1 | 性能测量方法论 | 预热、重复和异常值说明。 |
| 2 | perf 工具使用 | 采集 cycles、instructions、cache miss 和热点。 |
| 3 | 内存带宽瓶颈分析 | 根据 cache miss 与规模变化判断访存瓶颈。 |
| 4 | 循环展开与软件流水 | 对比展开前后耗时。 |
| 5 | 数据对齐与预取 | 使用对齐分配并记录变化。 |
| 6 | RVV 与 OpenBLAS 集成 | 可选与单线程 OpenBLAS 对比。 |

## 实验环境

| 项目 | 要求 |
| --- | --- |
| 主机环境 | 可整理报告 |
| 目标板 | LicheePi 4A |
| 目标系统 | RevyOS |
| 硬件连接 | 无额外外设 |
| 软件依赖 | `perf`、`gcc`、`make`、可选 OpenBLAS |

## 硬件连接或部署关系

| 模块 | 引脚/接口 | 连接说明 | 注意事项 |
| --- | --- | --- | --- |
| LicheePi 4A | CPU/PMU | 在板端运行 `perf` | 权限不足时用 `sudo` 或记录限制 |

## 实验任务

### 任务 1：建立基线

使用第 5.3 讲 `rvv_base` 版本，固定 `256x256x256` 或板端可稳定运行的规模。

### 任务 2：采集 perf

记录 `cycles`、`instructions`、`cache-misses`、`branches` 和热点函数。

### 任务 3：优化并复测

实现 `rvv_aligned`、`rvv_transpose_b` 或 `rvv_unroll`，每次只改一项并复测。

## 实验步骤

```bash
cd ~/rvv-labs/lab5_3
mkdir -p results/perf
perf stat -e cycles,instructions,cache-misses,branches,branch-misses \
  ./bench_sgemm --impl rvv_base --m 256 --n 256 --k 256 --repeat 5 \
  2>&1 | tee results/perf/base_stat.txt
```

```bash
perf record -g ./bench_sgemm --impl rvv_base --m 256 --n 256 --k 256 --repeat 3
perf report
```

```bash
for impl in rvv_base rvv_aligned rvv_unroll; do
  perf stat -e cycles,instructions,cache-misses \
    ./bench_sgemm --impl $impl --m 256 --n 256 --k 256 --repeat 5 \
    2>&1 | tee results/perf/${impl}.txt
done
```

## 运行验证

| 验证项 | 预期现象 | 是否通过 |
| --- | --- | --- |
| perf stat | 能输出 cycles/instructions/cache-misses |  |
| perf record | 能生成 `perf.data` 或记录权限限制 |  |
| 正确性 | 优化前后仍输出 `PASS` |  |
| 对比表 | 至少包含 3 组实现的耗时和加速比 |  |

## 验收记录

| 记录项 | 内容 |
| --- | --- |
| 运行命令 | `perf stat ... ./bench_sgemm ...` |
| 关键输出 | cycles、instructions、cache-misses、耗时、GFLOPS |
| 截图或照片 | `perf report` 热点截图或文本摘要 |
| 异常处理 | 无 perf 权限时记录错误并使用程序内计时替代 |

## 常见问题

| 现象 | 可能原因 | 处理方式 |
| --- | --- | --- |
| `No permission to enable cycles event` | perf 权限限制 | 使用 `sudo` 或记录限制。 |
| 优化后结果错误 | 分块边界或转置索引错误 | 先在小矩阵上校验。 |
| cache miss 数据难解释 | 事件支持有限或噪声大 | 结合规模变化和热点函数分析。 |

## 提交要求

- 实验记录：基线、优化项、测试规模、重复次数。
- 运行截图：`perf stat` 和热点摘要。
- 源码或配置文件：优化前后版本。
- 简短说明：每项优化是否有效及原因。
