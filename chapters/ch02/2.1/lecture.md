# 2.1 交叉编译全流程：从源码到板端可执行文件

## 本讲目标

- 能区分 host（编译主机）、target（运行设备）和 sysroot（目标根文件系统），并说明交叉编译与本地编译的差异。
- 能查询交叉编译器目标三元组与 sysroot 路径，填写三要素对照表。
- 能使用交叉编译器构建最小 C 程序，确认产物为 RISC-V ELF，并通过 SCP 部署到开发板运行。
- 能区分"在 host 编译成功"和"在 target 运行成功"两个验收环节。
- 能使用 `readelf` 查看 ELF 头、程序头中的关键字段（Machine、Class、Type、LOAD 段）。
- 能使用 `ldd` 查看动态可执行文件的共享库依赖，并解释与交叉编译的关系。

## 前置条件

- 已完成第一章全部节与实验：主机已通过 `ruyi` 安装 RISC-V Linux 工具链，开发板可 SSH/SCP 登录。
- 了解 ELF（Executable and Linkable Format）是 Linux 常用可执行文件格式。

## 知识简介

### host、target、sysroot 与交叉编译

嵌入式 Linux 开发很少在开发板上直接编译大型程序。更常见的流程是：在性能更好的 PC（host）上生成二进制，再部署到开发板（target）执行。当 host 与 target 的 CPU 架构不同时，这一过程称为交叉编译（cross compilation）。

工具链除了编译器，还需要目标系统的头文件和库。这些文件按目标系统的目录结构组织，合称为 sysroot（system root）。编译时编译器在 sysroot 中查找 `stdio.h`、C 库等；链接时也按目标 ABI 链接其中的库。若误用主机 sysroot，可能生成能在主机运行、却无法在板端运行的程序。

图 2-1：交叉编译流程

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

### 课程第一个完整交付闭环

编辑源码 → `make` 交叉编译 → `scp` 上传 → 板端 `chmod +x` → 执行并观察输出。示例工程位于 `chapters/ch02/code/hello/`，包含 `main.c` 与 Makefile。

主机上即使编译通过，也不能用 `./hello` 验证业务逻辑——x86_64 主机无法运行 RISC-V 二进制。正确验收必须在板端执行。

图 2-2：编译到运行流水线

```text
main.c  ──make──>  hello (RISC-V ELF)
                         │
                    scp 上传
                         ▼
              板端 ~/hello ──./hello──>  stdout
```

### 读懂 ELF：readelf 与 ldd

编译产物不仅是"能运行的文件"，还包含元数据：架构、字节序、入口地址、段布局、动态链接信息等。`readelf` 直接解析 ELF 结构；`ldd` 则针对动态链接程序，列出运行时需要的共享库。

课程中常用场景：上传板端前在 host 用 `readelf -h` 确认 Machine 字段为 RISC-V；用 `ldd` 确认依赖的 `libc.so.6` 等是否能在板端找到。静态链接程序 `ldd` 可能显示 "not a dynamic executable"，属正常。

图 2-3：ELF 分析维度

```text
hello (ELF)
 ├─ readelf -h   → Class, Machine, Type, Entry
 ├─ readelf -l   → LOAD 段、对齐、权限
 └─ ldd          → libc.so.6 => /lib/...
```

## 环境准备

| 项目 | 要求 | 检查方式 |
| --- | --- | --- |
| host 架构 | 记录主机 CPU 架构 | `uname -m` |
| target 架构 | 板端为 RISC-V 64 位 | `ssh <user>@<board-ip> uname -m` |
| 交叉编译器 | 前缀通常为 `riscv64-unknown-linux-gnu-` | `command -v riscv64-unknown-linux-gnu-gcc` |
| 目标三元组 | 与 Linux 用户态匹配 | `riscv64-unknown-linux-gnu-gcc -dumpmachine` |
| 工程目录 | `chapters/ch02/code/hello/` | `ls main.c Makefile` |
| 板端登录 | SSH 免密或可用密码，家目录可写 | `ssh <user>@<board-ip> true` |
| 分析工具 | host 有 `readelf`、`file` | `command -v readelf` |

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

不同安装方式下 sysroot 路径不同。先查询编译器默认搜索路径：

```bash
riscv64-unknown-linux-gnu-gcc -print-sysroot
riscv64-unknown-linux-gnu-gcc -v -E - < /dev/null 2>&1 | sed -n '/COLLECT_GCC/,/COMPILER_PATH/p'
```

若 `-print-sysroot` 为空，工具链可能使用内置默认路径或通过 `--sysroot=` 在编译时指定。记录本机实际路径；课程 Makefile 在需要时应显式传入 `SYSROOT`。

### 步骤 4：对比 host 与 target 运行环境

在主机上查看自身 shell：

```bash
file /bin/bash
ldd /bin/bash | head -n 3
```

在板端执行同样命令。观察动态链接器路径、库目录与架构差异。板端动态链接器通常位于 `/lib` 或 `/lib64` 下，且为 RISC-V ELF。

### 步骤 5：填写对照表

根据以上命令，完成 host、target、sysroot 三列对照：机器名称、架构、操作系统、工具链前缀、sysroot 路径、典型用途。

### 步骤 6：查看源码与 Makefile

```bash
cd chapters/ch02/code/hello
sed -n '1,20p' main.c
cat Makefile
```

`CROSS_COMPILE` 默认为 `riscv64-unknown-linux-gnu-`。若本机前缀不同，可在命令行覆盖：

```bash
make CROSS_COMPILE=<你的前缀>
```

### 步骤 7：交叉编译

```bash
make clean
make
file hello
```

`file hello` 应包含 `RISC-V` 和 `ELF`。若显示 `x86-64`，说明调用了错误的编译器。

### 步骤 8：上传到板端并运行

将 `<user>`、`<board-ip>` 替换为实际值：

```bash
scp hello <user>@<board-ip>:~/
ssh <user>@<board-ip> 'chmod +x ~/hello && file ~/hello'
ssh <user>@<board-ip> './hello'
```

预期输出：

```text
Hello, RISC-V Linux!
```

保存从 `make` 到 `./hello` 的终端输出，作为后续实验与项目的部署模板。

### 步骤 9：ELF 头信息

```bash
readelf -h hello
```

重点关注：

| 字段 | 含义 | 课程期望 |
| --- | --- | --- |
| Class | ELF 位数 | `ELF64` |
| Data | 字节序 | 小端 `LSB` 常见 |
| Machine | CPU 架构 | `RISC-V` |
| Type | 文件类型 | `EXEC` 或 `DYN` |

### 步骤 10：程序头（段布局）

```bash
readelf -l hello | sed -n '1,25p'
```

观察 `LOAD` 段：哪些部分映射到内存、是否可执行、对齐方式。理解"文件中的段"与"运行时的内存区域"对应关系即可，不要求手工计算地址。

### 步骤 11：节头（可选深入）

```bash
readelf -S hello | head -n 20
```

找到 `.text`（代码）、`.data`（已初始化数据）、`.bss`（未初始化数据）等节，建立与 C 源码编译结果的对应概念。

### 步骤 12：动态库依赖

```bash
ldd hello
file hello
```

若 `file` 显示 "dynamically linked"，`ldd` 应列出 `libc.so.6` 等。若显示 "statically linked"，`ldd` 不适用，应在实验记录中注明链接方式。

### 步骤 13：与板端对比（可选）

```bash
ssh <user>@<board-ip> 'ldd ~/hello; ls -l /lib/libc.so.6 2>/dev/null || ls -l /lib64/libc.so.6'
```

确认板端存在 `ldd` 报告的库路径，或解释 musl/glibc 差异（以所用镜像为准）。

## 课堂练习

1. 说明以下说法哪句正确，并改正错误句：
   - "在 x86_64 笔记本上 `gcc hello.c` 得到的程序，可以直接 scp 到 LicheePi 4A 运行。"
   - "sysroot 就是开发板上的整个 `/` 目录，必须每次从板子完整复制。"
   - "交叉编译时，编译器应链接 target 的 C 库，而不是 host 的 C 库。"
2. 若 `scp` 成功但板端执行提示 `cannot execute binary file`，应依次检查哪些项目？写出至少三条。
3. `readelf -h` 中 `Machine` 为 `Advanced Micro Devices X86-64` 时，说明构建过程哪里出了问题？应如何修正？

## 运行验证

| 验证项 | 预期现象 | 记录 |
| --- | --- | --- |
| host 架构 | `uname -m` 为 x86_64 等 | 保存输出 |
| target 架构 | `uname -m` 为 `riscv64` | 保存输出 |
| 三元组 | 含 `riscv64` 与 `linux-gnu` | 保存 `-dumpmachine` |
| sysroot | 得到路径或说明如何指定 | 记录路径或命令 |
| 对照表 | 三要素均有具体实例 | 提交表格 |
| 本地 `make` | 生成 `hello` | 保存输出 |
| `file hello`（主机） | RISC-V ELF | 保存输出 |
| SCP | 上传无报错 | 记录命令 |
| `file ~/hello`（板端） | 仍为 RISC-V ELF | 保存输出 |
| 板端运行 | 打印问候字符串 | 保存输出 |
| `readelf -h` | Machine 为 RISC-V | 保存关键行 |
| `readelf -l` | 可见 LOAD 段 | 保存片段 |
| `ldd` | 列出依赖或注明静态链接 | 保存输出 |
| 判断 | 能说明架构与链接方式 | 书面结论 |

## 常见问题

### 混淆 Linux 工具链与裸机工具链

`riscv64-unknown-elf-gcc` 面向无操作系统环境；本课程用户态程序应使用带 `linux-gnu` 的工具链。

### `-print-sysroot` 为空

部分工具链将 sysroot 打包在内部，或通过 wrapper 脚本注入。应结合 `riscv64-unknown-linux-gnu-gcc -v` 输出和 `ruyi` 包说明查找；不要因此改用主机 `gcc`。

### `cannot execute binary file`

常见原因：架构不匹配（用了主机 gcc）、上传损坏、或误传目录。用两端 `file` 命令核对。

### `Permission denied` / `No such file or directory`

可执行位未设置：板端执行 `chmod +x ~/hello`。若已 chmod 仍报 `No such file or directory`，可能是动态链接器路径与板端不一致——用 `ldd` 在板端检查依赖。

### host 上 `ldd` 显示 "not found"

交叉编译的动态程序常链接到 sysroot 中的库，host 上 `ldd` 可能找不到目标路径。这不一定表示板端不能运行；应在板端再执行 `ldd`，或检查编译时 sysroot 是否正确。

### SCP 慢或中断

检查网络；大文件可改用 `rsync -avz`。本实验文件很小，通常一次 `scp` 即可。

### `readelf` 与 `objdump -f` 的区别

两者都能查看头信息；课程统一用 `readelf` 观察 ELF 结构，`objdump -d` 则更多用于反汇编指令。

### 在板端编译是否算交叉编译

在板端本机编译属于 native compile，可用于小实验，但速度慢、环境难统一。课程主线仍在 host 交叉编译，以保持一致的工程方式。

## 本讲成果

- host / target / sysroot 对照表。
- 交叉编译器目标三元组与 sysroot 路径记录。
- 板端可运行的 Hello World。
- 一套可复用的「编译 → scp → 运行」命令记录。
- `readelf -h` / `readelf -l` 关键字段摘录。
- `ldd` 或静态链接说明。
- 对应实验 2.1 的完整终端日志与验收材料。
