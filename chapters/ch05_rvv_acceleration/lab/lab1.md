# 实验 5.1：RVV memcpy/saxpy 初测

## 对应讲次

| 项目 | 内容 |
| --- | --- |
| 章节 | 第 5 章 RISC-V 向量扩展（RVV）加速编程 |
| 讲次 | 第 1 讲 RISC-V 向量扩展体系结构 |
| 课程主题 | RVV 汇编、stripmining、标量/RVV 基准对比 |
| 实验类型 | 必做实验 |

大纲讲次原文：第1讲 RISC-V 向量扩展体系结构。
大纲实验原文：编写 RVV 汇编版 memcpy/saxpy，对比标量版本性能
大纲知识点原文：RVV 设计哲学与优势；向量寄存器与 CSR；向量指令分类；LicheePi 4A 的 RVV 能力；RVV 汇编基础；GCC/Clang RVV 编译支持。

## 实验目标

- 确认 LicheePi 4A + RevyOS 上的 CPU、内核、编译器和 RVV 编译选项。
- 编写标量版和 RVV 版 `saxpy` 或 `memcpy`，完成输出正确性校验。
- 记录多轮耗时并计算加速比。

## 对应知识点

| # | 知识点 | 本实验中的验证方式 |
| --- | --- | --- |
| 1 | RVV 设计哲学与优势 | 使用 `vl` 循环处理任意长度数组。 |
| 2 | 向量寄存器与 CSR | 通过汇编中的 `vsetvli` 理解 `vl/vtype`。 |
| 3 | 向量指令分类 | 使用加载、运算、存储指令。 |
| 4 | LicheePi 4A 的 RVV 能力 | 记录 CPU 和工具链信息。 |
| 5 | RVV 汇编基础 | 编写或阅读 `.S` 文件。 |
| 6 | GCC/Clang RVV 编译支持 | 使用 `-march=rv64gcv -mabi=lp64d` 编译。 |

## 实验环境

| 项目 | 要求 |
| --- | --- |
| 主机环境 | 可 SSH/SCP 到 LicheePi 4A |
| 目标板 | LicheePi 4A |
| 目标系统 | RevyOS |
| 硬件连接 | 无额外外设，保持稳定供电和散热 |
| 软件依赖 | `gcc`、`make`、`time`，可选 `perf` |

## 硬件连接或部署关系

| 模块 | 引脚/接口 | 连接说明 | 注意事项 |
| --- | --- | --- | --- |
| LicheePi 4A | Type-C/电源 | 供电并启动 RevyOS | 长时间测试注意散热 |
| 主机 | Ethernet/Wi-Fi | SSH 登录板端 | 保持网络稳定 |

## 实验任务

### 任务 1：建立工程

创建 `saxpy_scalar.c`、`saxpy_rvv.S`、`bench_saxpy.c`、`Makefile` 和 `results/`。

### 任务 2：实现与校验

标量版按 `y[i] = a * x[i] + y[i]` 实现；RVV 版使用 `vsetvli` 分块。比较两个输出数组的最大误差。

### 任务 3：性能记录

测试 `N=65536/1048576/4194304`，每个规模预热 1 次、正式运行 5 次，记录均值和加速比。

## 实验步骤

```bash
mkdir -p ~/rvv-labs/lab5_1/results
cd ~/rvv-labs/lab5_1
uname -a | tee results/env.txt
lscpu | tee -a results/env.txt
gcc --version | tee -a results/env.txt
```

```bash
make clean
make CFLAGS="-O3 -march=rv64gcv -mabi=lp64d"
./bench_saxpy --n 1024 --check
./bench_saxpy --n 1048576 --repeat 5 | tee results/saxpy_1m.txt
```

如果 RVV 汇编暂时无法编译，仍需运行标量版并保存编译错误：

```bash
make scalar
./bench_saxpy_scalar --n 1048576 --repeat 5 | tee results/scalar_only.txt
make rvv 2>&1 | tee results/rvv_build_error.txt
```

## 运行验证

| 验证项 | 预期现象 | 是否通过 |
| --- | --- | --- |
| 编译 | 生成 `bench_saxpy` |  |
| 正确性 | `--check` 输出 `PASS` 或最大误差小于阈值 |  |
| 耗时 | 输出标量/RVV 多轮耗时 |  |
| 加速比 | 能计算 `scalar_time / rvv_time` |  |

## 验收记录

| 记录项 | 内容 |
| --- | --- |
| 运行命令 | `./bench_saxpy --n 1048576 --repeat 5` |
| 关键输出 | 标量耗时、RVV 耗时、最大误差、加速比 |
| 截图或照片 | 终端截图或 `results/saxpy_1m.txt` |
| 异常处理 | RVV 不可用时提交编译错误和标量基线 |

## 常见问题

| 现象 | 可能原因 | 处理方式 |
| --- | --- | --- |
| `Illegal instruction` | RVV 版本或运行平台不匹配 | 确认在 LicheePi 4A 板端运行。 |
| 输出不一致 | 尾量或指针步进错误 | 打印小规模数组定位。 |
| 加速比小于 1 | 数据太小或访存瓶颈 | 增大 N 并记录解释。 |

## 提交要求

- 实验记录：环境信息、编译命令、测试规模、重复次数。
- 运行截图：正确性 `PASS` 和性能输出。
- 源码或配置文件：标量版、RVV 版、Makefile。
- 简短说明：加速比是否稳定，以及可能瓶颈。
