# 2.6 选读：CoreMark 对比算力边界

## 本讲目标

- 了解 CoreMark 嵌入式基准测试的用途与局限。
- 能在板端获取、构建并运行 CoreMark，读懂分数含义。
- 能说明基准分数与真实应用性能（I/O、外设、网络）之间的区别。

## 前置条件

- 完成 2.5，熟悉交叉编译与板端部署。
- 本讲为**选读**，不阻塞主线实验验收。

## 知识简介

CoreMark 是 EEMBC 发布的轻量级 CPU 基准，主要测试列表操作、矩阵运算、状态机等核心逻辑，结果常以 **CoreMark/MHz** 或绝对 **CoreMark 分数** 报告。它适合粗略对比不同 MCU/SoC 或不同频率下的 CPU 效率，**不能**代表 GPIO 抖动、网络吞吐、存储速度等系统级表现。

在 LicheePi 4A 上运行 CoreMark 的价值：建立“这块板子 CPU 大概什么量级”的直觉，为后续是否用软解码、图像处理等留参考。课程综合项目验收不依赖 CoreMark 分数。

图 2-6 建议展示：

```text
CoreMark 源码
   ├─ 交叉编译 → coremark.exe（板端）
   ├─ 板端运行 → Iterations / Score
   └─ 对比资料 → 仅作参考，非实验硬指标
```

## 环境准备

| 项目 | 要求 | 检查方式 |
| --- | --- | --- |
| 源码 | 官方或课程提供的 CoreMark 压缩包 | 解压后含 `core_main.c` |
| 工具链 | 与课程一致 | 能生成 RISC-V ELF |
| 板端 | 可 SSH，运行时间数分钟可接受 | `ssh` 登录 |

## 操作步骤

### 步骤 1：获取源码

从 EEMBC 官方或课程镜像站获取 CoreMark。解压后进入源码目录，阅读 `README.md` 与 `core_main.c` 顶部说明。

### 步骤 2：选择编译方式

CoreMark 通常通过 `Makefile` 指定 `XCFLAGS`、`PORT_DIR`、`ITERATIONS` 等。课程板端为 Linux 用户态，应使用 Linux/POSIX 移植而非裸机移植。示例（以实际源码 Makefile 为准）：

```bash
export CC=riscv64-unknown-linux-gnu-gcc
# 按源码 README 执行，例如：
make PORT_DIR=linux XCFLAGS="-O2" link
file coremark.exe   # 或实际产物名
```

具体目标文件名因版本而异，以本机 `make` 输出为准。

### 步骤 3：部署到板端

```bash
scp coremark.exe <user>@<board-ip>:~/
ssh <user>@<board-ip> 'chmod +x ~/coremark.exe && ./coremark.exe'
```

记录输出的 **Iterations**、**Total time**、**CoreMark** 分数。若支持多线程编译，可对比单线程与多线程（选做）。

### 步骤 4：归一化理解（选做）

若已知 CPU 频率 `f` MHz，可计算 CoreMark/MHz = 分数 / f。与公开资料对比时注意：编译选项、libc、散热、是否绑核都会影响结果。

### 步骤 5：写简短结论

用三五句话回答：CoreMark 高分是否意味着 MQTT 或 DHT22 实验一定更流畅？为什么？

## 课堂练习

列举三项**不会**被 CoreMark 单独反映、但在本课程实验中很重要的系统能力。

## 运行验证

| 验证项 | 预期现象 | 记录 |
| --- | --- | --- |
| 交叉编译 | 生成板端可执行文件 | 保存 `file` 输出 |
| 板端运行 | 打印 CoreMark 摘要 | 保存完整输出 |
| 结论 | 能说明基准边界 | 书面 3–5 句 |

## 常见问题

### 编译报错找不到 `time.h` 或 POSIX 接口

说明选错了 PORT（裸机而非 Linux）。改选 Linux/POSIX 移植目录。

### 板端运行极慢或 CPU 占用 100% 很久

CoreMark 设计上会跑满 CPU 一段时间，属正常；不要在生产系统上长时间占用若与他人共用板子。

### 分数与网上截图差很多

编译器版本、`-O` 级别、glibc/musl、是否固定 CPU 频率都会导致差异。只作同环境前后对比，不追求绝对名次。

## 本讲成果

- 可选：一份 CoreMark 板端运行日志。
- 可选：一段关于“算力基准 vs 课程应用”的简短评述。
