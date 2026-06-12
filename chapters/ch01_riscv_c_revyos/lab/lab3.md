# 实验 1.3：远程 GDB 调试与 Git 分支实践

## 对应讲次

| 项目 | 内容 |
| --- | --- |
| 章节 | 第 1 章 RISC-V C语言开发与调试 |
| 讲次 | 第 3 讲 |
| 课程主题 | 调试技术与版本控制 |
| 实验类型 | 必做实验 |

大纲讲次原文：第3讲 调试技术与版本控制。
大纲实验原文：远程 GDB 调试 LicheePi 4A 上的程序 + Git 分支管理实践
大纲知识点原文：GDB 本地调试；GDB 远程交叉调试；段错误与内存问题排查；strace/ltrace 系统调用追踪；Git 基础操作；Git 分支管理与协作。

## 实验目标

- 编译带调试符号的 RISC-V 程序。
- 使用 `gdbserver` 和 `gdb-multiarch` 完成远程断点调试。
- 使用 `strace` 或 core dump 观察程序异常行为。
- 使用 Git 分支保存源码、日志和实验说明。

## 对应知识点

| # | 知识点 | 本实验中的验证方式 |
| --- | --- | --- |
| 1 | GDB 本地调试 | 主机端加载符号，执行 `break`、`next`、`print` |
| 2 | GDB 远程交叉调试 | 板端运行 `gdbserver :2345`，主机端 `target remote` |
| 3 | 段错误与内存问题排查 | 构造可控错误，记录 core dump 或 GDB backtrace |
| 4 | strace/ltrace 系统调用追踪 | 生成 `strace` 日志并解释关键系统调用 |
| 5 | Git 基础操作 | `git add`、`commit`、`log` 保存实验过程 |
| 6 | Git 分支管理与协作 | 创建实验分支并模拟一次合并或冲突处理 |

## 实验环境

| 项目 | 要求 |
| --- | --- |
| 主机环境 | Linux x86_64，已安装交叉编译器、`gdb-multiarch`、`git`、`ssh`、`scp` |
| 目标板 | LicheePi 4A |
| 目标系统 | RevyOS |
| 硬件连接 | 主机可 SSH 登录开发板 |
| 软件依赖 | 板端安装 `gdbserver`、`strace`，可选 `ltrace` |

## 硬件连接或部署关系

| 模块 | 引脚/接口 | 连接说明 | 注意事项 |
| --- | --- | --- | --- |
| 主机 GDB | TCP 2345 | 连接板端 `gdbserver` | 保证局域网可达 |
| LicheePi 4A | RevyOS 用户态 | 运行待调试程序 | 调试程序需有执行权限 |
| Git 仓库 | 主机工程目录 | 保存源码、日志、实验记录 | 不提交无关构建垃圾 |

## 实验任务

### 任务 1：编译调试版本

使用 `-g -O0 -Wall` 构建可调试程序，并用 `readelf` 确认调试段存在。

### 任务 2：完成远程 GDB 调试

板端运行 `gdbserver`，主机端连接后设置断点、单步、查看变量和调用栈。

### 任务 3：记录系统调用和 Git 提交

使用 `strace` 生成日志，创建 Git 分支并提交实验结果。

## 实验步骤

1. 主机端编译调试版本。

```bash
make clean
make CFLAGS="-g -O0 -Wall" all
riscv64-unknown-linux-gnu-readelf -S build/hello | grep debug
scp build/hello <user>@<board-ip>:/tmp/debug-demo
```

2. 板端安装并启动调试服务。

```bash
ssh <user>@<board-ip> 'sudo apt update && sudo apt install -y gdbserver strace'
ssh <user>@<board-ip> 'chmod +x /tmp/debug-demo && gdbserver :2345 /tmp/debug-demo'
```

3. 主机端连接 GDB。

```bash
gdb-multiarch build/hello
```

在 GDB 中执行：

```text
set architecture riscv:rv64
target remote <board-ip>:2345
break main
continue
next
print argc
backtrace
quit
```

4. 生成 strace 日志并取回主机。

```bash
ssh <user>@<board-ip> 'strace -o /tmp/debug-demo.strace /tmp/debug-demo'
mkdir -p logs
scp <user>@<board-ip>:/tmp/debug-demo.strace logs/
```

5. Git 分支提交。

```bash
git switch -c lab-1-3-gdb
git status
git add src Makefile logs README.md
git commit -m "lab: remote gdb debugging on licheepi4a"
git log --oneline --decorate -3
```

## 运行验证

能命中断点、单步执行、查看变量和 backtrace，并提交包含实验说明的 Git commit。

| 验证项 | 预期现象 | 是否通过 |
| --- | --- | --- |
| 调试符号 | `readelf` 能看到 `.debug_*` |  |
| GDB 连接 | `target remote` 后停在程序入口或断点 |  |
| 变量查看 | `print` 能显示变量值 |  |
| 调用栈 | `backtrace` 能显示函数调用链 |  |
| strace | 日志中能看到 `execve`、`write`、`exit_group` 等调用 |  |
| Git 提交 | `git log` 显示实验提交 |  |

## 验收记录

| 记录项 | 内容 |
| --- | --- |
| 运行命令 | `gdbserver :2345`、`gdb-multiarch`、`strace`、`git commit` |
| 关键输出 | 断点命中行、变量值、backtrace、Git commit hash |
| 截图或照片 | GDB 调试界面、Git log |
| 异常处理 | 记录端口占用、符号路径、core dump 设置问题 |

## 常见问题

| 现象 | 可能原因 | 处理方式 |
| --- | --- | --- |
| GDB 断点无法命中 | 源码和二进制不匹配 | 重新编译并传输同一个二进制 |
| `target remote` 失败 | IP/端口错误或 gdbserver 已退出 | 重新启动 gdbserver，检查 `ss -lntp` |
| 源码路径找不到 | 编译路径与当前路径不同 | GDB 中使用 `directory` 或 `set substitute-path` |
| Git 提交包含构建产物 | `.gitignore` 不完整 | 忽略 `build/`、临时日志以外的大文件 |

## 提交要求

- 实验记录：GDB 命令序列、断点位置、变量值、backtrace。
- 运行截图：板端 `gdbserver`、主机端 GDB、Git log。
- 源码或配置文件：调试程序源码、Makefile、实验说明。
- 简短说明：解释 strace 日志中 3 个关键系统调用。

