# 实验 1.2：Makefile 工程模板与 Hello World 部署

## 对应讲次

| 项目 | 内容 |
| --- | --- |
| 章节 | 第 1 章 RISC-V C语言开发与调试 |
| 讲次 | 第 2 讲 |
| 课程主题 | 交叉编译工具链与工程构建 |
| 实验类型 | 必做实验 |

大纲讲次原文：第2讲 交叉编译工具链与工程构建。
大纲实验原文：编写 Makefile 工程模板，交叉编译 Hello World 并部署到 LicheePi 4A
大纲知识点原文：交叉编译原理；RuyiSDK RISC-V 工具链安装与验证；第一个 RISC-V 程序；GCC 编译选项精讲；Makefile 编写与工程管理；CMake 入门。

## 实验目标

- 建立 `src/`、`include/`、`build/` 组成的最小工程目录。
- 使用 RISC-V 交叉编译器生成 LicheePi 4A 可运行程序。
- 使用 `file`、`readelf`、可选 `ldd` 解释目标文件属性。
- 使用 Makefile 完成 `all`、`clean`、`deploy` 三类目标。

## 对应知识点

| # | 知识点 | 本实验中的验证方式 |
| --- | --- | --- |
| 1 | 交叉编译原理 | 主机端编译，板端运行，记录 host/target 架构差异 |
| 2 | RuyiSDK 工具链安装与验证 | 执行 `riscv64-unknown-linux-gnu-gcc --version` |
| 3 | 第一个 RISC-V 程序 | 编译并运行 Hello World |
| 4 | GCC 编译选项精讲 | 对比 `-O0`、`-O2`、`-static` 生成结果 |
| 5 | Makefile 编写与工程管理 | 使用 Makefile 管理构建、清理和部署 |
| 6 | CMake 入门 | 可选完成 out-of-source build 并与 Makefile 结果对比 |

## 实验环境

| 项目 | 要求 |
| --- | --- |
| 主机环境 | Linux x86_64，已安装 RuyiSDK 工具链、`make`、`file`、`ssh`、`scp` |
| 目标板 | LicheePi 4A |
| 目标系统 | RevyOS |
| 硬件连接 | 开发板已联网，主机可 SSH 登录 |
| 软件依赖 | `riscv64-unknown-linux-gnu-gcc`、`readelf`、可选 `cmake` |

## 硬件连接或部署关系

| 模块 | 引脚/接口 | 连接说明 | 注意事项 |
| --- | --- | --- | --- |
| 主机 | 网络 | 通过 SSH/SCP 连接开发板 | 保持与开发板同网段 |
| LicheePi 4A | Ethernet/Wi-Fi | 接收并运行交叉编译产物 | 记录板端 IP |
| 程序部署目录 | `/tmp/rv-course` 或 `/opt/rv-course` | 初学阶段建议先部署到 `/tmp` | `/opt` 需要 sudo 权限 |

## 实验任务

### 任务 1：创建最小工程

创建源码、头文件、构建目录，编写输出系统信息提示的 C 程序。

### 任务 2：编写 Makefile

Makefile 至少支持 `all`、`clean`、`deploy`，编译参数包含 `-Wall -O2 -march=rv64gc -mabi=lp64d`。

### 任务 3：部署和解释 ELF

把程序传到 LicheePi 4A 运行，并解释 `file/readelf` 输出。

## 实验步骤

1. 创建工程目录和源码。

```bash
mkdir -p rv-hello/src rv-hello/include rv-hello/build
cd rv-hello
```

`src/main.c` 至少包含一行能识别实验的输出，例如课程名、编译时间或目标板名称。

2. 编写 Makefile 后构建。

```bash
make clean
make all
file build/hello
riscv64-unknown-linux-gnu-readelf -h build/hello
```

3. 对比动态链接和静态链接。

```bash
riscv64-unknown-linux-gnu-gcc -O2 -march=rv64gc -mabi=lp64d src/main.c -o build/hello-dyn
riscv64-unknown-linux-gnu-gcc -O2 -static -march=rv64gc -mabi=lp64d src/main.c -o build/hello-static
ls -lh build/hello-dyn build/hello-static
file build/hello-dyn build/hello-static
```

4. 部署并运行。

```bash
scp build/hello <user>@<board-ip>:/tmp/rv-hello
ssh <user>@<board-ip> 'chmod +x /tmp/rv-hello && /tmp/rv-hello'
```

5. 可选：用 CMake 构建同一程序，构建目录必须在源码目录外或 `build-cmake/` 中。

```bash
cmake -S . -B build-cmake -DCMAKE_TOOLCHAIN_FILE=cmake/riscv64-linux.cmake
cmake --build build-cmake
```

## 运行验证

板端能运行程序；host 端能用 `file`、`readelf` 解释目标架构、ABI 和链接方式。

| 验证项 | 预期现象 | 是否通过 |
| --- | --- | --- |
| 工具链 | GCC 输出版本信息 |  |
| Makefile 构建 | `make all` 生成 `build/hello` |  |
| ELF 解释 | `file` 显示 RISC-V ELF64 |  |
| 板端运行 | 输出实验自定义文本 |  |
| 链接对比 | 静态版本体积明显大于动态版本 |  |

## 验收记录

| 记录项 | 内容 |
| --- | --- |
| 运行命令 | `make all`、`file build/hello`、`readelf -h`、`ssh ... /tmp/rv-hello` |
| 关键输出 | `Machine: RISC-V`、程序输出文本、二进制大小 |
| 截图或照片 | 主机构建输出、板端运行输出 |
| 异常处理 | 记录 `Exec format error`、库缺失或 Makefile 错误的处理 |

## 常见问题

| 现象 | 可能原因 | 处理方式 |
| --- | --- | --- |
| 编译器命令不存在 | 工具链未安装或 PATH 未配置 | 重新加载 RuyiSDK 环境或使用绝对路径 |
| `readelf` 不是 RISC-V 版本 | 调用了主机工具 | 使用 `riscv64-unknown-linux-gnu-readelf` |
| 板端不能执行 | 权限不足或架构错误 | `chmod +x`，再用 `file` 确认架构 |
| 静态链接失败 | 缺少静态库或工具链限制 | 记录失败信息，动态链接作为默认路径 |

## 提交要求

- 实验记录：工程目录树、工具链版本、`file/readelf` 关键输出。
- 运行截图：主机构建和板端运行。
- 源码或配置文件：`src/main.c`、`Makefile`、可选 CMake toolchain file。
- 简短说明：解释动态/静态链接差异。

