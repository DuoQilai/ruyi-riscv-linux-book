# 实验 5.2：RVV intrinsics 图像处理加速

## 对应讲次

| 项目 | 内容 |
| --- | --- |
| 章节 | 第 5 章 RISC-V 向量扩展（RVV）加速编程 |
| 讲次 | 第 2 讲 RVV Intrinsics 编程 |
| 课程主题 | `riscv_vector.h`、加载/存储、掩码、图像处理 |
| 实验类型 | 必做实验 |

大纲讲次原文：第2讲 RVV Intrinsics 编程。
大纲实验原文：使用 RVV intrinsics 编写图像处理算法加速，对比标量版本性能并在 LicheePi 4A 上记录加速比
大纲知识点原文：Intrinsics 编程模型；向量长度设置与加载；向量算术与逻辑运算；向量存储与段操作；向量归约与排列；向量与标量混合编程。

## 实验目标

- 使用 PGM 灰度图或随机灰度数组作为输入。
- 实现阈值化或 RGB 转灰度的标量版和 RVV intrinsics 版。
- 对输出文件做逐字节校验，并记录耗时。

## 对应知识点

| # | 知识点 | 本实验中的验证方式 |
| --- | --- | --- |
| 1 | Intrinsics 编程模型 | 包含 `riscv_vector.h` 并使用向量类型。 |
| 2 | 向量长度设置与加载 | 每轮根据剩余像素设置 `vl`。 |
| 3 | 向量算术与逻辑运算 | 阈值比较、加权或掩码选择。 |
| 4 | 向量存储与段操作 | 将处理结果写回输出图像。 |
| 5 | 向量归约与排列 | 可选统计像素和或最大值。 |
| 6 | 向量与标量混合编程 | 用 C 代码处理文件 I/O 和计时。 |

## 实验环境

| 项目 | 要求 |
| --- | --- |
| 主机环境 | 可准备测试图片并传输到板端 |
| 目标板 | LicheePi 4A |
| 目标系统 | RevyOS |
| 硬件连接 | 无额外外设 |
| 软件依赖 | `gcc`、`make`、可选 `python3` 或 ImageMagick 生成 PGM |

## 硬件连接或部署关系

| 模块 | 引脚/接口 | 连接说明 | 注意事项 |
| --- | --- | --- | --- |
| LicheePi 4A | 文件系统 | 输入图片放到 `~/rvv-labs/lab5_2/input.pgm` | 图片格式固定为 PGM/P5 便于解析 |

## 实验任务

### 任务 1：准备输入

准备一张 `640x480` 或 `1280x720` 的 PGM 灰度图；没有图片时程序生成随机灰度数组。

### 任务 2：实现算法

实现 `threshold_scalar()` 和 `threshold_rvv()`，阈值固定为 128，输出像素为 0 或 255。

### 任务 3：验证和计时

逐字节比较 `out_scalar.pgm` 与 `out_rvv.pgm`，记录两种实现的耗时和加速比。

## 实验步骤

```bash
mkdir -p ~/rvv-labs/lab5_2/results
cd ~/rvv-labs/lab5_2
make clean
make CFLAGS="-O3 -march=rv64gcv -mabi=lp64d"
```

```bash
./threshold --input input.pgm --impl scalar --output out_scalar.pgm --repeat 10
./threshold --input input.pgm --impl rvv --output out_rvv.pgm --repeat 10
cmp out_scalar.pgm out_rvv.pgm
./threshold --input input.pgm --check --repeat 10 | tee results/threshold.txt
```

观察向量指令：

```bash
gcc -O3 -march=rv64gcv -mabi=lp64d -S threshold_rvv.c -o threshold_rvv.s
grep -n "vset\\|vle\\|vse" threshold_rvv.s | tee results/vector_asm.txt
```

## 运行验证

| 验证项 | 预期现象 | 是否通过 |
| --- | --- | --- |
| 编译 | `threshold` 可执行文件生成 |  |
| 输出一致 | `cmp` 无输出或程序输出 `PASS` |  |
| 图像可检查 | 生成 `out_scalar.pgm` 和 `out_rvv.pgm` |  |
| 性能记录 | 输出标量/RVV 耗时和加速比 |  |

## 验收记录

| 记录项 | 内容 |
| --- | --- |
| 运行命令 | `./threshold --input input.pgm --check --repeat 10` |
| 关键输出 | 最大差异、标量耗时、RVV 耗时、加速比 |
| 截图或照片 | 输出图或终端截图 |
| 异常处理 | intrinsics 不兼容时提交 API 差异和标量结果 |

## 常见问题

| 现象 | 可能原因 | 处理方式 |
| --- | --- | --- |
| `riscv_vector.h` 找不到 | 工具链不支持 RVV intrinsics | 更换工具链或记录为待补测。 |
| `cmp` 不一致 | 尾量处理错误 | 用宽高不能整除 `vl` 的小图测试。 |
| 输出图片打不开 | PGM 头部写错 | 检查 magic、宽高、最大值和换行。 |

## 提交要求

- 实验记录：图片尺寸、阈值、重复次数。
- 运行截图：`PASS`、耗时输出、可选输出图。
- 源码或配置文件：标量/RVV 实现、Makefile。
- 简短说明：是否出现 API 兼容问题。
