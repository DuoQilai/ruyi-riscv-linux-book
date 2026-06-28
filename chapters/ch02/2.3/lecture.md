# 2.3 readelf、ldd 观察二进制

## 本讲目标

- 能使用 `readelf` 查看 ELF 头、程序头与节头中的关键字段。
- 能使用 `ldd` 查看动态可执行文件的共享库依赖。
- 能根据输出判断二进制是否面向 RISC-V Linux，并解释与交叉编译的关系。

## 前置条件

- 完成 2.2，已有板端可运行的 `hello` 或本机构建的同名文件。
- 了解 ELF（Executable and Linkable Format）是 Linux 常用可执行文件格式。

## 知识简介

编译产物不仅是“能运行的文件”，还包含元数据：架构、字节序、入口地址、段布局、动态链接信息等。`readelf` 直接解析 ELF 结构；`ldd` 则针对动态链接程序，列出运行时需要的共享库。

课程中常用场景：上传板端前在 host 用 `readelf -h` 确认 Machine 字段为 RISC-V；用 `ldd` 确认依赖的 `libc.so.6` 等是否能在板端找到。静态链接程序 `ldd` 可能显示 “not a dynamic executable”，这属于正常现象。

图 2-3 建议展示：

```text
hello (ELF)
 ├─ readelf -h   → Class, Machine, Type, Entry
 ├─ readelf -l   → LOAD 段、对齐、权限
 └─ ldd          → libc.so.6 => /lib/...
```

## 环境准备

| 项目 | 要求 | 检查方式 |
| --- | --- | --- |
| 示例二进制 | `chapters/ch02/code/hello/hello` | `test -f hello` |
| 工具 | host 上有 `readelf`、`file` | `command -v readelf` |
| 板端 | 可选，对比板端系统库 | SSH 可用 |

## 操作步骤

### 步骤 1：ELF 头信息

在 `chapters/ch02/code/hello/` 目录：

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

### 步骤 2：程序头（段布局）

```bash
readelf -l hello | sed -n '1,25p'
```

观察 `LOAD` 段：哪些部分映射到内存、是否可执行、对齐方式。理解“文件中的段”与“运行时的内存区域”对应关系即可，不要求手工计算地址。

### 步骤 3：节头（可选深入）

```bash
readelf -S hello | head -n 20
```

找到 `.text`（代码）、`.data`（已初始化数据）、`.bss`（未初始化数据）等节，建立与 C 源码编译结果的对应概念。

### 步骤 4：动态库依赖

```bash
ldd hello
file hello
```

若 `file` 显示 “dynamically linked”，`ldd` 应列出 `libc.so.6` 等。若显示 “statically linked”，`ldd` 不适用，应在实验记录中注明链接方式。

### 步骤 5：与板端对比（可选）

```bash
ssh <user>@<board-ip> 'ldd ~/hello; ls -l /lib/libc.so.6 2>/dev/null || ls -l /lib64/libc.so.6'
```

确认板端存在 `ldd` 报告的库路径，或解释 musl/glibc 差异（以所用镜像为准）。

## 课堂练习

`readelf -h` 中 `Machine` 为 `Advanced Micro Devices X86-64` 时，说明构建过程哪里出了问题？应如何修正？

## 运行验证

| 验证项 | 预期现象 | 记录 |
| --- | --- | --- |
| `readelf -h` | Machine 为 RISC-V | 保存关键行 |
| `readelf -l` | 可见 LOAD 段 | 保存片段 |
| `ldd` | 列出依赖或注明静态链接 | 保存输出 |
| 判断 | 能说明架构与链接方式 | 书面结论 |

## 常见问题

### host 的 `readelf` 能否分析 RISC-V 文件

可以。`readelf` 解析文件格式，不要求与本机架构相同。

### `ldd` 在 host 上显示 “not found”

交叉编译的动态程序常链接到 sysroot 中的库，host 上 `ldd` 可能找不到目标路径。这不一定表示板端不能运行；应在板端再执行 `ldd`，或检查编译时 sysroot 是否正确。

### `readelf` 与 `objdump -f` 的区别

两者都能查看头信息；课程统一用 `readelf` 观察 ELF 结构，`objdump -d` 则更多用于反汇编指令。

## 本讲成果

- 一份 `readelf -h` / `readelf -l` 关键字段摘录。
- 一份 `ldd` 或静态链接说明。
- 对应实验 2.3 的观察记录。
