# 5.1 RISC-V 向量扩展体系结构

## 对应大纲

大纲讲次原文：第1讲 RISC-V 向量扩展体系结构。
大纲知识点原文：RVV 设计哲学与优势；向量寄存器与 CSR；向量指令分类；LicheePi 4A 的 RVV 能力；RVV 汇编基础；GCC/Clang RVV 编译支持。

RVV 设计哲学、向量寄存器与 CSR、向量指令分类、LicheePi 4A 的 RVV 能力、RVV 汇编基础、GCC/Clang RVV 编译支持。

## 目标

学生能解释 C910 RVV 能力，理解向量长度无关的编程方式，编写 RVV 汇编版 memcpy/saxpy 并与标量版本对比。

## 知识点

| # | 知识点 | 内容 |
| --- | --- | --- |
| 1 | RVV 设计哲学与优势 | 对比固定 SIMD，说明 VLA 代码如何适配不同 VLEN。 |
| 2 | 向量寄存器与 CSR | 介绍 `v0-v31`、`vtype`、`vl`、`vstart`、`vxsat`、`vlenb`。 |
| 3 | 向量指令分类 | 归纳加载存储、算术逻辑、比较、掩码、归约和排列指令。 |
| 4 | LicheePi 4A 的 RVV 能力 | 说明 TH1520/C910、RevyOS、工具链和 RVV 兼容性检查。 |
| 5 | RVV 汇编基础 | 讲解 `vsetvli`、`vle.v`、`vse.v`、`vadd.vv` 和 stripmining。 |
| 6 | GCC/Clang RVV 编译支持 | 使用 `-march=rv64gcv -mabi=lp64d`、`-S`、自动向量化报告。 |

## 讲授要点

- RVV 程序不应假设一次能处理固定数量元素，而是每轮用 `vsetvli` 获取当前 `vl`。
- stripmining 循环的核心是“剩余元素数 -> 设置 `vl` -> 处理 `vl` 个元素 -> 指针前移”。
- `vtype` 同时描述 SEW 和 LMUL；SEW 影响元素宽度，LMUL 影响寄存器组占用。
- 讲 C910 能力时要区分硬件能力、内核支持和编译器支持，实验报告必须记录三者。
- 汇编入门只要求读懂和改写小片段，复杂算法优先在后续用 intrinsics 实现。

## 操作或演示

1. 在板端记录环境：

```bash
uname -a
lscpu
cat /proc/cpuinfo | head -80
gcc --version
```

2. 观察标量 C 的汇编输出：

```bash
gcc -O2 -S saxpy_scalar.c -o saxpy_scalar.s
sed -n '1,120p' saxpy_scalar.s
```

3. 尝试 RVV 编译选项并保存诊断信息：

```bash
gcc -O2 -march=rv64gcv -mabi=lp64d -S saxpy_rvv.S -o saxpy_rvv.s
gcc -O3 -march=rv64gcv -mabi=lp64d bench_saxpy.c saxpy_rvv.S -o bench_saxpy
```

4. 演示 stripmining 伪代码：

```c
for (size_t n_left = n; n_left > 0; ) {
    size_t vl = vsetvl(n_left);
    /* load vl elements, compute, store */
    n_left -= vl;
}
```

## 运行验证

| 验证项 | 命令或方法 | 预期现象 |
| --- | --- | --- |
| 环境记录 | `uname -a && gcc --version` | 能保存内核和编译器版本。 |
| 编译验证 | `make scalar rvv` | 标量和 RVV 目标均生成；若 RVV 编译失败，记录错误。 |
| 正确性验证 | `./bench_saxpy --check` | 输出 `PASS`，最大误差在阈值内。 |
| 性能记录 | `./bench_saxpy --n 1048576 --repeat 5` | 输出标量耗时、RVV 耗时和加速比。 |

## 常见问题

| 现象 | 可能原因 | 处理方式 |
| --- | --- | --- |
| `unknown ISA extension 'v'` | GCC 不支持 RVV 目标 | 更换 RuyiSDK 工具链或记录为待补测。 |
| 运行 `Illegal instruction` | 二进制和硬件/内核支持不匹配 | 确认在 LicheePi 4A 上运行，重新检查 `-march`。 |
| saxpy 误差不为 0 | 浮点计算顺序变化 | 使用绝对误差或相对误差阈值，不用逐字节比较。 |

## 本讲成果

- 能画出 RVV stripmining 循环流程。
- 能编译或至少诊断 RVV 汇编示例。
- 完成一次标量/RVV saxpy 或 memcpy 的正确性和耗时初测记录。
