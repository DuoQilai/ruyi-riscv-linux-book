# 1.2 交叉编译工具链与工程构建

## 对应大纲

大纲讲次原文：第2讲 交叉编译工具链与工程构建。
大纲知识点原文：交叉编译原理；RuyiSDK RISC-V 工具链安装与验证；第一个 RISC-V 程序；GCC 编译选项精讲；Makefile 编写与工程管理；CMake 入门。

交叉编译原理、RuyiSDK RISC-V 工具链安装与验证、第一个 RISC-V 程序、GCC 编译选项、Makefile、CMake 入门。

## 目标

学生能在 host 端编译 C 程序，传输到 LicheePi 4A，并用 `file`、`readelf` 解释 ELF 属性、目标架构、ABI 和链接方式。

## 知识点

| # | 知识点 | 本讲说明 |
| --- | --- | --- |
| 1 | 交叉编译原理 | 区分 host、build、target，理解为什么在 x86_64 主机上生成 riscv64 程序 |
| 2 | RuyiSDK 工具链安装与验证 | 安装 RISC-V GNU 工具链，检查 gcc、binutils、sysroot |
| 3 | 第一个 RISC-V 程序 | 完成 Hello World 编译、传输、运行和结果记录 |
| 4 | GCC 编译选项精讲 | 使用 `-O0/-O2/-Os`、`-march=rv64gc`、`-mabi=lp64d`、`-static` |
| 5 | Makefile 编写与工程管理 | 使用变量、规则、自动变量、伪目标管理多文件工程 |
| 6 | CMake 入门 | 使用 out-of-source build 和 toolchain file 构建同一程序 |

## 讲授要点

- 交叉编译失败时先看三件事：编译器前缀是否正确、目标 ABI 是否匹配、运行时依赖是否能在 RevyOS 上找到。
- `file` 和 `readelf` 是判断“编出来的到底是什么”的第一组工具，必须在部署前使用。
- 动态链接程序体积小，但依赖板端库；静态链接程序部署简单，但体积更大，并可能受 glibc 静态链接限制。
- Makefile 用于解释构建规则的基本模型，CMake 用于后续较大工程；本讲不追求复杂框架，先保证可复现构建。

## 操作或演示

1. 验证工具链。

```bash
ruyi --version
riscv64-unknown-linux-gnu-gcc --version
riscv64-unknown-linux-gnu-readelf --version
```

2. 编译最小程序。

```bash
mkdir -p rv-hello/src build
cat > rv-hello/src/main.c <<'EOF'
#include <stdio.h>
int main(void) {
    puts("Hello from LicheePi 4A + RevyOS");
    return 0;
}
EOF
riscv64-unknown-linux-gnu-gcc -O2 -march=rv64gc -mabi=lp64d rv-hello/src/main.c -o build/hello
file build/hello
riscv64-unknown-linux-gnu-readelf -h build/hello
```

3. 部署到板端运行。

```bash
scp build/hello <user>@<board-ip>:/tmp/hello
ssh <user>@<board-ip> 'chmod +x /tmp/hello && /tmp/hello'
```

4. 演示 Makefile 的核心结构：`CC`、`CFLAGS`、`SRC`、`OBJ`、`clean`、`deploy`，并说明变量和自动变量 `$@`、`$<`、`$^` 的含义。

## 运行验证

| 验证项 | 命令 | 预期现象 |
| --- | --- | --- |
| 编译器可用 | `riscv64-unknown-linux-gnu-gcc --version` | 输出 GNU GCC 版本 |
| ELF 架构 | `file build/hello` | 包含 `ELF 64-bit` 和 `RISC-V` |
| ELF 头 | `readelf -h build/hello` | `Machine: RISC-V`，Class 为 `ELF64` |
| 板端运行 | `ssh <user>@<board-ip> /tmp/hello` | 输出 Hello 文本 |
| Makefile | `make clean all` | 重新生成目标文件和可执行文件 |

## 常见问题

| 现象 | 可能原因 | 处理方式 |
| --- | --- | --- |
| 板端提示 `Exec format error` | 编译成了主机架构二进制 | 用 `file` 检查，确认使用 RISC-V 交叉编译器 |
| 运行时报动态库缺失 | 动态链接依赖板端不存在 | 使用 `ldd` 检查依赖，安装库或调整 sysroot，也可临时尝试静态链接 |
| `-march` 报错 | 工具链版本或 ISA 字符串不支持 | 先使用课程默认 `rv64gc/lp64d`，记录工具链版本 |
| Makefile 每次都全量编译 | 依赖关系写错或目标文件路径不稳定 | 检查目标、依赖和自动变量使用 |

## 本讲成果

- 一个可在 LicheePi 4A + RevyOS 运行的 Hello World 程序。
- 一份 `file/readelf` 观察记录，能解释 ELF64、RISC-V、ABI 和链接方式。
- 一个可复用的 Makefile 工程模板，可扩展到后续 GPIO/UART 实验。

