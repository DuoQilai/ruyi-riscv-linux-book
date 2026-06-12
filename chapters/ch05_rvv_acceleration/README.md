# 第五章 RISC-V 向量扩展（RVV）加速编程

## 本章目标

- 理解 RVV 的向量长度无关设计、寄存器/CSR、指令类别和 stripmining 循环模式。
- 能在 LicheePi 4A + RevyOS 上确认 C910 的 RVV 能力，并选择合适的 GCC/Clang 编译选项。
- 能分别使用标量 C、RVV 汇编和 RVV intrinsics 编写同一算法的不同实现。
- 能对 memcpy、saxpy、图像处理和 SGEMM 等任务做正确性校验、耗时统计和加速比记录。
- 能使用 `rdcycle`、`time`、`perf stat`、`perf record` 等工具定位向量化程序的瓶颈。
- 完成一份 RVV 算法加速实验报告，说明测试环境、数据规模、编译选项、结果和改进方向。

## 前置条件

- 已完成第 1 章开发环境、交叉编译、SSH/SCP、GDB 和 Shell 自动化部署。
- 已具备 C 语言数组、指针、结构体、Makefile、基础汇编和 Linux 命令行能力。
- 已准备 LicheePi 4A 开发板、RevyOS 系统、可用网络连接和主机端编辑/构建环境。
- 建议已了解第 4 章中的文件读写、日志和性能记录方式，便于整理实验报告。

## 知识简介

RVV 是 RISC-V 面向数据并行计算的标准扩展。与固定宽度 SIMD 不同，RVV 程序通过 `vl` 描述本轮实际处理的元素数量，通过 `vsetvli` 让同一段代码适配不同 VLEN 的处理器。LicheePi 4A 搭载 TH1520，核心为玄铁 C910，课程中统一将它作为 RVV 实验目标平台。由于工具链、内核和芯片实现可能涉及 RVV 0.7.1 与 RVV 1.0 兼容问题，本章实验要求先确认板端编译器和运行环境，再记录实际可用选项，不把未实测结果写成固定结论。

本章从体系结构和汇编入门开始，逐步过渡到 intrinsics、常见算法向量化和 perf 调优。所有实验都要求保留标量版本和 RVV 版本，先验证输出一致，再记录耗时、吞吐量或加速比。

## 环境准备

| 项目 | 要求 | 检查方式 |
| --- | --- | --- |
| 主机环境 | Linux/macOS/WSL 均可，能通过 SSH 访问 LicheePi 4A | `ssh sipeed@licheepi-ip` |
| 目标环境 | LicheePi 4A + RevyOS，CPU 为 TH1520/C910 | `uname -a`、`lscpu`、`cat /proc/cpuinfo` |
| 工具依赖 | `gcc`/`g++`、`make`、`binutils`、`perf`、`time` | `gcc --version`、`perf --version` |
| 编译选项 | 根据工具链确认 `-march=rv64gcv` 或板端实际支持的 RVV 选项 | `gcc -march=rv64gcv -dM -E - < /dev/null` |
| 数据目录 | 建议使用 `~/rvv-labs` 保存源码、二进制和日志 | `mkdir -p ~/rvv-labs/results` |
| 硬件连接 | 本章无需额外外设，保持散热和稳定供电 | 长时间测试时观察温度和降频现象 |

## 学习路径

| 讲次 | 主题 | 学习重点 | 对应实验 |
| --- | --- | --- | --- |
| 5.1 | RISC-V 向量扩展体系结构 | VLA、寄存器/CSR、指令分类、C910 RVV 能力、汇编和编译选项 | RVV memcpy/saxpy 初测 |
| 5.2 | RVV Intrinsics 编程 | `riscv_vector.h`、向量类型、加载/存储、算术、归约、掩码 | 图像处理算法 RVV 加速 |
| 5.3 | 常用算法向量化优化 | memcpy/memset、SGEMM、卷积、ReLU、Sobel、int8 推理 | RVV SGEMM |
| 5.4 | 向量化性能分析与调优 | `rdcycle`、`perf`、Roofline、展开、对齐、OpenBLAS 对比 | SGEMM 性能瓶颈分析 |

## 运行验证

| 验证项 | 预期现象 | 记录 |
| --- | --- | --- |
| RVV 能力确认 | 能记录 CPU、内核、GCC 版本和可用 `-march` 选项 | `results/env.txt` |
| 正确性校验 | 标量/RVV 输出一致，误差在实验设定阈值内 | `PASS` 或最大误差 |
| 耗时统计 | 每个规模至少预热 1 次、正式运行 5 次，记录均值 | `results/*.csv` |
| 加速比 | 能计算 `scalar_time / rvv_time`，并说明异常结果 | 实验报告表格 |
| perf 分析 | 至少记录 cycles、instructions、cache-misses 或热点函数 | `perf stat`/`perf report` 摘要 |

## 常见问题

### 编译器不接受 `-march=rv64gcv`

现象：编译时报 unknown ISA extension 或 intrinsics 类型不存在。

原因：板端 GCC 版本、RVV 版本或发行版默认工具链不匹配。

处理：先记录 `gcc --version`，尝试课程提供的 RuyiSDK 工具链；若只能使用标量工具链，本章实验可先完成自动向量化观察和标量基线，RVV 结果标记为“待补测”。

### RVV 程序运行非法指令

现象：运行时报 `Illegal instruction`。

原因：二进制使用的向量扩展与当前内核/硬件/工具链 ABI 不匹配。

处理：重新确认 `-march`、`-mabi=lp64d`、目标板型号和运行位置；不要在非 LicheePi 4A 主机上直接运行 RVV 二进制。

### 加速比不稳定

现象：同一程序多次运行差异很大。

原因：数据规模太小、CPU 调频、缓存热身不足、后台任务干扰。

处理：增加数据规模和重复次数，固定测试命令，记录温度/频率，使用中位数或均值并说明测试条件。

## 本章成果

- 一套可复用的 RVV 实验工程目录，包含标量版、RVV 版、Makefile 和测试脚本。
- 四份实验记录：memcpy/saxpy、图像处理、SGEMM、perf 调优。
- 一份 RVV 性能对比报告，至少包含环境信息、正确性校验、耗时、加速比和瓶颈分析。
- 可提交材料：源码、运行命令、`results/*.csv`、关键截图或终端日志、简短总结。
