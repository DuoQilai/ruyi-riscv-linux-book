# 5.2 RVV Intrinsics 编程

## 对应大纲

大纲讲次原文：第2讲 RVV Intrinsics 编程。
大纲知识点原文：Intrinsics 编程模型；向量长度设置与加载；向量算术与逻辑运算；向量存储与段操作；向量归约与排列；向量与标量混合编程。

Intrinsics 编程模型、向量长度设置与加载、向量算术与逻辑运算、向量存储与段操作、向量归约与排列、向量与标量混合编程。

## 目标

学生能使用 `riscv_vector.h` 编写可读的 RVV C 函数，完成图像处理或数组计算片段的向量化，并与标量版本做正确性和耗时对比。

## 知识点

| # | 知识点 | 内容 |
| --- | --- | --- |
| 1 | Intrinsics 编程模型 | `riscv_vector.h`、向量类型和 `__riscv_` 前缀 API。 |
| 2 | 向量长度设置与加载 | `vsetvl`、`vle8/vle32`，根据剩余元素动态设置 `vl`。 |
| 3 | 向量算术与逻辑运算 | 加减乘、浮点、FMA、按位逻辑和带掩码运算。 |
| 4 | 向量存储与段操作 | `vse`、跨步 load/store、分段 load/store 与图像通道处理。 |
| 5 | 向量归约与排列 | `vredsum`、`vredmax`、`vrgather`、`vslideup/down`。 |
| 6 | 向量与标量混合编程 | C 循环、边界处理、`volatile`、优化屏障和函数接口设计。 |

## 讲授要点

- Intrinsics 的价值是让向量逻辑留在 C/C++ 层，同时避免完全依赖编译器自动向量化。
- 讲解函数命名时把“操作、操作数类型、向量类型、掩码版本”拆开读，不要求学生死记全表。
- 图像处理适合演示 RVV：输入输出容易校验，数据并行明显，能看到标量/RVV 两个版本的结构差异。
- 分段加载适合 RGB/RGBA 交错数据，但初学实验可以先从灰度图或单通道数组入手。
- 尾量处理仍交给 `vl`，不要额外写复杂的尾循环，除非用于对比固定 SIMD 思路。

## 操作或演示

1. 建立工程文件：

```bash
mkdir -p ~/rvv-labs/intrinsics
cd ~/rvv-labs/intrinsics
touch gray_scalar.c gray_rvv.c bench_gray.c Makefile
```

2. 标量版本处理单通道图像：

```c
void threshold_scalar(const unsigned char *src, unsigned char *dst, int n, unsigned char t) {
    for (int i = 0; i < n; ++i) dst[i] = src[i] > t ? 255 : 0;
}
```

3. RVV 版本按 `vl` 分块处理，课堂根据工具链补充对应 intrinsics 名称：

```c
#include <riscv_vector.h>

void threshold_rvv(const unsigned char *src, unsigned char *dst, int n, unsigned char t) {
    for (int i = 0; i < n; ) {
        size_t vl = __riscv_vsetvl_e8m1(n - i);
        /* load, compare, merge, store */
        i += vl;
    }
}
```

4. 编译和查看汇编：

```bash
gcc -O3 -march=rv64gcv -mabi=lp64d -S gray_rvv.c -o gray_rvv.s
grep -n "vset\\|vle\\|vse" gray_rvv.s
```

## 运行验证

| 验证项 | 命令或方法 | 预期现象 |
| --- | --- | --- |
| 头文件验证 | `echo '#include <riscv_vector.h>' | gcc -march=rv64gcv -mabi=lp64d -E -` | 能预处理通过。 |
| 正确性 | `./bench_gray --check --width 640 --height 480` | 标量和 RVV 输出一致。 |
| 耗时 | `./bench_gray --repeat 10` | 输出每轮耗时和均值。 |
| 汇编观察 | `grep 'vle\\|vse\\|vset' gray_rvv.s` | 能看到向量指令。 |

## 常见问题

| 现象 | 可能原因 | 处理方式 |
| --- | --- | --- |
| intrinsics 名称不匹配 | 工具链版本不同 | 以本机 `riscv_vector.h` 为准，记录 API 差异。 |
| RVV 版本比标量慢 | 数据规模小或内存带宽主导 | 增大输入，分开记录冷启动和热缓存结果。 |
| 输出边缘有错误 | 图像宽高和步长处理混淆 | 明确 `n = width * height`，不要把 stride 忽略。 |

## 本讲成果

- 一个 RVV intrinsics 图像或数组处理函数。
- 一份标量/RVV 输出一致性记录。
- 一份包含编译选项、耗时和汇编截图的实验笔记。
