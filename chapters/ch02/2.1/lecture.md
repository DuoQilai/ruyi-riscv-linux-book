# 2.1 host、target、sysroot 概念

## 本讲目标

- 能区分 host（编译主机）、target（运行设备）和 sysroot（目标根文件系统）。
- 能说明交叉编译与本地编译的差异，以及课程为何在主机上编译、在板端运行。
- 能根据本机工具链输出，填写 host/target/sysroot 对照表并指出对应路径或命令。

## 前置条件

- 已完成第一章全部节与实验。
- 主机已通过 `ruyi` 安装 RISC-V Linux 工具链。
- 开发板可 SSH 登录，用于确认 target 环境。

## 知识简介

嵌入式 Linux 开发很少在开发板上直接编译大型程序。更常见的流程是：在性能更好的 PC（host）上生成二进制，再部署到开发板（target）执行。当 host 与 target 的 CPU 架构不同时，这一过程称为交叉编译（cross compilation）。

工具链除了编译器，还需要目标系统的头文件和库。这些文件按目标系统的目录结构组织，合称为 sysroot（system root）。编译时，编译器在 sysroot 中查找 `stdio.h`、C 库等；链接时，也按目标 ABI 链接其中的库。若误用主机 sysroot，可能生成能在主机运行、却无法在板端运行的程序。

图 2-1 建议展示：

```text
host（x86_64 Linux PC）
  ├─ 编辑器、make、ruyi
  ├─ riscv64-unknown-linux-gnu-gcc
  └─ sysroot（RISC-V 根文件系统片段）
            ↓ 交叉编译
      RISC-V ELF 可执行文件
            ↓ scp / 其他部署
target（LicheePi 4A + 课程镜像如 RevyOS）
  └─ ./your-program 在板端运行
```

## 环境准备

| 项目 | 要求 | 检查方式 |
| --- | --- | --- |
| host 架构 | 记录主机 CPU 架构 | `uname -m` |
| target 架构 | 板端为 RISC-V 64 位 | `ssh <user>@<board-ip> uname -m` |
| 交叉编译器 | 前缀通常为 `riscv64-unknown-linux-gnu-` | `command -v riscv64-unknown-linux-gnu-gcc` |
| 目标三元组 | 与 Linux 用户态匹配 | `riscv64-unknown-linux-gnu-gcc -dumpmachine` |

## 操作步骤

### 步骤 1：确认 host 与 target

在主机执行：

```bash
uname -m
hostname
```

在板端执行：

```bash
uname -m
hostname
cat /etc/os-release | head -n 5
```

主机常见结果为 `x86_64`，板端为 `riscv64`。两者不同，说明需要交叉编译，而不能直接在主机上运行板端二进制。

### 步骤 2：查看交叉编译器目标三元组

```bash
riscv64-unknown-linux-gnu-gcc -dumpmachine
riscv64-unknown-linux-gnu-gcc --version | head -n 1
```

输出应包含 `riscv64` 和 `linux-gnu`。若出现 `unknown-elf` 且无 `linux-gnu`，通常表示裸机工具链，不适合本课程 Linux 应用开发。

### 步骤 3：定位 sysroot

不同安装方式下 sysroot 路径不同。可先查询编译器默认搜索路径：

```bash
riscv64-unknown-linux-gnu-gcc -print-sysroot
riscv64-unknown-linux-gnu-gcc -v -E - < /dev/null 2>&1 | sed -n '/COLLECT_GCC/,/COMPILER_PATH/p'
```

若 `-print-sysroot` 为空，工具链可能使用内置默认路径或通过 `--sysroot=` 在编译时指定。记录本机实际路径；课程 Makefile 在需要时应显式传入 `SYSROOT`。

### 步骤 4：对比 host 与 target 上的 `file` 与 `ldd` 环境

在主机上查看自身 shell：

```bash
file /bin/bash
ldd /bin/bash | head -n 3
```

在板端执行同样命令。观察动态链接器路径、库目录与架构差异。板端动态链接器通常位于 `/lib` 或 `/lib64` 下，且为 RISC-V ELF。

### 步骤 5：填写对照表

根据以上命令，完成 host、target、sysroot 三列对照：机器名称、架构、操作系统、工具链前缀、sysroot 路径、典型用途。

## 课堂练习

说明以下说法哪句正确，并改正错误句：

1. “在 x86_64 笔记本上 `gcc hello.c` 得到的程序，可以直接 scp 到 LicheePi 4A 运行。”
2. “sysroot 就是开发板上的整个 `/` 目录，必须每次从板子完整复制。”
3. “交叉编译时，编译器应链接 target 的 C 库，而不是 host 的 C 库。”

## 运行验证

| 验证项 | 预期现象 | 记录 |
| --- | --- | --- |
| host 架构 | `uname -m` 为 x86_64 等 | 保存输出 |
| target 架构 | `uname -m` 为 `riscv64` | 保存输出 |
| 三元组 | 含 `riscv64` 与 `linux-gnu` | 保存 `-dumpmachine` |
| sysroot | 得到路径或说明如何指定 | 记录路径或命令 |
| 对照表 | 三要素均有具体实例 | 提交表格 |

## 常见问题

### `-print-sysroot` 为空

部分工具链将 sysroot 打包在内部，或通过 wrapper 脚本注入。应结合 `riscv64-unknown-linux-gnu-gcc -v` 输出和 `ruyi` 包说明查找；不要因此改用主机 `gcc`。

### 混淆 Linux 工具链与裸机工具链

`riscv64-unknown-elf-gcc` 面向无操作系统环境；本课程用户态程序应使用带 `linux-gnu` 的工具链。

### 在板端编译是否算交叉编译

在板端本机编译属于 native compile，可用于小实验，但速度慢、环境难统一。课程主线仍在 host 交叉编译，以保持与实验 1、综合项目一致的工程方式。

## 本讲成果

- 一份 host / target / sysroot 对照表。
- 交叉编译器目标三元组与 sysroot 路径记录。
- 对应实验 2.1 的验收材料。
