# 5.4 向量化性能分析与调优

## 对应大纲

大纲讲次原文：第4讲 向量化性能分析与调优。
大纲知识点原文：性能测量方法论；perf 工具使用；内存带宽瓶颈分析；循环展开与软件流水；数据对齐与预取；RVV 与 OpenBLAS 集成。

性能测量方法论、perf 工具使用、内存带宽瓶颈分析、循环展开与软件流水、数据对齐与预取、RVV 与 OpenBLAS 集成。

## 目标

学生能用一致的测量方法分析第 5.3 讲的 RVV SGEMM 或图像算法，提出并验证至少两项优化，形成性能对比报告。

## 知识点

| # | 知识点 | 内容 |
| --- | --- | --- |
| 1 | 性能测量方法论 | 预热、重复、均值/中位数、`rdcycle`/`rdtime` 和噪声控制。 |
| 2 | perf 工具使用 | `perf stat`、`perf record`、`perf report` 和热点解读。 |
| 3 | 内存带宽瓶颈分析 | Roofline 思路、STREAM 基准、算术强度和带宽上限。 |
| 4 | 循环展开与软件流水 | 编译器展开、手工展开、寄存器压力和指令级并行。 |
| 5 | 数据对齐与预取 | 128 字节对齐、cache line、手动预取和误用风险。 |
| 6 | RVV 与 OpenBLAS 集成 | RVV 后端 OpenBLAS、GEMM/GEMV 调用和库对比方法。 |

## 讲授要点

- 性能数据必须可复现：固定输入、固定命令、记录版本、重复运行。
- `perf stat` 适合看总体事件，`perf record/report` 适合定位热点函数。
- Roofline 是解释工具，不是精确预测；课堂上用它帮助判断“该优化计算还是访存”。
- 展开和预取都有副作用，优化必须用数据证明，不凭感觉保留。
- 与 OpenBLAS 对比时要说明库版本、线程数和编译选项，避免把多线程结果和单线程手写代码混在一起。

## 操作或演示

1. 固定测试规模并保存环境：

```bash
mkdir -p ~/rvv-labs/results
uname -a > ~/rvv-labs/results/env.txt
gcc --version >> ~/rvv-labs/results/env.txt
```

2. 用 `perf stat` 采集整体事件：

```bash
perf stat -e cycles,instructions,cache-misses,branches,branch-misses \
  ./bench_sgemm --m 256 --n 256 --k 256 --repeat 5
```

3. 用 `perf record` 查热点：

```bash
perf record -g ./bench_sgemm --m 256 --n 256 --k 256 --repeat 3
perf report
```

4. 对比优化前后：

```bash
./bench_sgemm --impl rvv_base --m 256 --n 256 --k 256 --repeat 5
./bench_sgemm --impl rvv_aligned --m 256 --n 256 --k 256 --repeat 5
./bench_sgemm --impl rvv_unroll --m 256 --n 256 --k 256 --repeat 5
```

## 运行验证

| 验证项 | 命令或方法 | 预期现象 |
| --- | --- | --- |
| 统计稳定性 | 同一命令重复 5 次 | 耗时波动可解释，异常值被标注。 |
| perf 总览 | `perf stat ...` | 能记录 cycles、instructions、cache miss。 |
| 热点定位 | `perf report` | 热点集中在预期计算函数。 |
| 优化对比 | base/aligned/unroll 三组结果 | 表格展示优化前后耗时和加速比。 |

## 常见问题

| 现象 | 可能原因 | 处理方式 |
| --- | --- | --- |
| `perf_event_paranoid` 限制 | 系统默认权限较严 | 使用 `sudo` 或记录无法采集的原因。 |
| 优化后变慢 | 寄存器压力、cache 行为变差 | 保留数据，回退该优化并解释。 |
| OpenBLAS 对比不公平 | 线程数或编译选项不同 | 设置 `OPENBLAS_NUM_THREADS=1` 并记录版本。 |

## 本讲成果

- 一份 `perf stat` 与热点分析记录。
- 至少两项优化尝试及其前后数据。
- 一份 RVV 性能分析报告，明确哪些数据为实测、哪些为待补测。
