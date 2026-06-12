# 第一章 RISC-V C语言开发与调试

本章围绕 LicheePi 4A + RevyOS 建立嵌入式 Linux C 开发闭环：从镜像烧录、首次登录、网络配置，到 RuyiSDK 交叉编译、远程调试、Git 记录和自动化部署。完成本章后，学生应能把一个 C 程序从主机端构建出来，可靠传输到开发板，运行、调试并注册为可管理服务。

## 本章目标

- 认识 RISC-V 生态、LicheePi 4A 硬件资源和 RevyOS 运行环境。
- 完成 RevyOS 镜像烧录、首次启动、网络配置、SSH/SCP 远程访问。
- 使用 RuyiSDK 工具链交叉编译 C 程序，并能解释 ELF 架构、ABI 和链接方式。
- 掌握 GDB/gdbserver、core dump、strace/ltrace 等基础调试手段。
- 使用 Git 记录实验过程，使用 Shell 脚本和 systemd 建立自动化部署流程。

## 前置条件

- 已具备基本 Linux 命令行能力，能使用 `cd`、`ls`、`mkdir`、`ssh`、`scp`、`sudo`。
- 了解 C 语言函数、指针、数组、头文件和编译链接的基本概念。
- 准备 LicheePi 4A、可靠电源、USB 数据线或串口线、microSD/eMMC 启动介质、HDMI 显示器或串口终端。
- 主机端建议使用 Linux x86_64 环境；如使用 macOS/Windows，应通过虚拟机、WSL 或远程 Linux 主机完成交叉编译。

## 知识简介

RISC-V 嵌入式开发与普通桌面 C 开发最大的差异在于 host/target 分离：程序通常在主机端编译，在目标板端运行。RevyOS 为 LicheePi 4A 提供 Linux 用户态环境，RuyiSDK 提供 RISC-V 工具链和软件包管理能力。学生需要同时理解板卡启动、网络访问、二进制格式、远程调试和服务管理，才能在后续外设、进程线程和网络项目中高效迭代。

## 环境准备

| 项目 | 要求 | 检查方式 |
| --- | --- | --- |
| 主机环境 | Linux x86_64，已安装 `git`、`make`、`cmake`、`ssh`、`scp`、`rsync`、`gdb-multiarch` | `uname -m`、`git --version`、`cmake --version` |
| 目标环境 | LicheePi 4A 运行 RevyOS，可通过串口/HDMI 登录 | `cat /etc/os-release`、`uname -a` |
| 工具依赖 | RuyiSDK RISC-V GNU 工具链、`gdbserver`、`strace`、`ltrace` | `riscv64-unknown-linux-gnu-gcc --version`、`gdbserver --version` |
| 硬件连接 | 稳定供电，串口或 HDMI 可用，开发板接入局域网 | 板端 `ip addr`，主机 `ping <板端IP>` |

## 学习路径

| 讲次 | 主题 | 学习重点 | 配套实验 |
| --- | --- | --- | --- |
| 1.1 | RISC-V 生态与开发环境搭建 | ISA、开发板、RevyOS 烧录、首次配置、SSH/SCP | 镜像烧录、网络配置与 SSH 登录 |
| 1.2 | 交叉编译工具链与工程构建 | host/target、RuyiSDK、GCC 选项、Makefile、CMake | Makefile 工程模板与 Hello World 部署 |
| 1.3 | 调试技术与版本控制 | GDB、gdbserver、core dump、strace/ltrace、Git 分支 | 远程 GDB 调试与 Git 分支实践 |
| 1.4 | Shell 编程与自动化部署 | Shell、参数处理、文本工具、SCP/rsync、systemd | 自动化部署脚本与 systemd 服务 |

## 运行验证

| 验证项 | 预期现象 | 记录 |
| --- | --- | --- |
| RevyOS 启动 | 串口或 HDMI 能进入登录提示，`cat /etc/os-release` 显示 RevyOS | 记录系统版本和内核版本 |
| 网络连通 | 主机能 `ping <板端IP>`，可 SSH 登录 | 记录 IP、网关、登录用户名 |
| 交叉编译 | `file build/hello` 显示 `RISC-V`，`readelf -h` 显示 `ELF64` | 记录 `Machine`、ABI、链接方式 |
| 板端运行 | `./hello` 在 LicheePi 4A 上输出预期文本 | 记录命令和输出 |
| 远程调试 | host 端 GDB 能连接 `gdbserver`，命中断点并查看变量 | 记录断点、backtrace |
| 自动部署 | 脚本能完成构建、传输、远程运行，`systemctl status` 能查看服务 | 记录服务状态和日志 |

## 本章成果

- 一份可复用的 RevyOS 环境配置记录，包含镜像版本、登录方式、网络信息和 SSH 配置。
- 一个最小 C 工程模板，包含 `src/`、`include/`、`Makefile`、可选 `CMakeLists.txt` 和部署脚本。
- 一份调试记录，包含 GDB 远程连接、core dump 或 strace/ltrace 的观察结果。
- 一个 systemd 服务示例，能在 LicheePi 4A 上启动、停止、查看状态并按需开机自启。

