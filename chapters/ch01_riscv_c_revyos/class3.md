# 1.3 调试技术与版本控制

## 对应大纲

大纲讲次原文：第3讲 调试技术与版本控制。
大纲知识点原文：GDB 本地调试；GDB 远程交叉调试；段错误与内存问题排查；strace/ltrace 系统调用追踪；Git 基础操作；Git 分支管理与协作。

GDB 本地调试、GDB 远程交叉调试、段错误与内存问题排查、strace/ltrace、Git 基础操作、Git 分支管理与协作。

## 目标

学生能对板端程序设置断点、单步调试、查看变量和调用栈，能用 core dump、strace/ltrace 辅助定位问题，并用 Git 分支记录实验过程。

## 知识点

| # | 知识点 | 本讲说明 |
| --- | --- | --- |
| 1 | GDB 本地调试 | 断点、单步、变量、backtrace、TUI 模式和 `.gdbinit` |
| 2 | GDB 远程交叉调试 | 板端 `gdbserver`，host 端 `gdb-multiarch`，远程连接和 sysroot |
| 3 | 段错误与内存问题排查 | core dump、`bt full`、AddressSanitizer 的适用边界 |
| 4 | strace/ltrace 系统调用追踪 | 观察文件、进程、网络和库函数调用 |
| 5 | Git 基础操作 | init/clone/add/commit/status/log/diff 和 `.gitignore` |
| 6 | Git 分支管理与协作 | branch/switch/merge/rebase、冲突解决和 PR 工作流 |

## 讲授要点

- 调试版本应使用 `-g -O0` 或较低优化级别，发布版本再使用 `-O2`；优化会改变变量可见性和单步体验。
- 远程调试分工清晰：目标板运行程序和 `gdbserver`，主机端加载符号并控制执行。
- core dump 适合事后分析，strace/ltrace 适合观察程序与系统边界的交互。
- Git 提交要小而清晰，实验记录、源码和配置应一起形成可复现证据。

## 操作或演示

1. 编译调试版本。

```bash
riscv64-unknown-linux-gnu-gcc -g -O0 -Wall src/main.c -o build/debug-demo
scp build/debug-demo <user>@<board-ip>:/tmp/debug-demo
```

2. 板端启动 `gdbserver`。

```bash
ssh <user>@<board-ip> 'gdbserver :2345 /tmp/debug-demo'
```

3. 主机端连接远程调试。

```bash
gdb-multiarch build/debug-demo
(gdb) set architecture riscv:rv64
(gdb) target remote <board-ip>:2345
(gdb) break main
(gdb) continue
(gdb) next
(gdb) print argc
(gdb) backtrace
```

4. 演示系统调用追踪和 Git 记录。

```bash
ssh <user>@<board-ip> 'strace -o /tmp/debug-demo.strace /tmp/debug-demo'
scp <user>@<board-ip>:/tmp/debug-demo.strace logs/
git switch -c lab-gdb-practice
git add src logs README.md
git commit -m "lab: record remote gdb practice"
```

## 运行验证

| 验证项 | 命令 | 预期现象 |
| --- | --- | --- |
| 调试符号 | `readelf -S build/debug-demo` | 能看到 `.debug_*` 段 |
| 远程连接 | `target remote <board-ip>:2345` | GDB 显示连接到远程目标 |
| 断点命中 | `break main`、`continue` | 程序停在 `main` |
| 调用栈 | `backtrace` | 能显示当前函数调用链 |
| Git 记录 | `git log --oneline --decorate -3` | 能看到实验提交和分支名称 |

## 常见问题

| 现象 | 可能原因 | 处理方式 |
| --- | --- | --- |
| GDB 找不到源码 | 调试符号中的路径和主机路径不一致 | 使用 `directory` 或 `set substitute-path` |
| 连接 `gdbserver` 超时 | 端口未监听、防火墙或 IP 错误 | 板端确认 `ss -lntp`，主机确认 IP 和端口 |
| 变量显示 `<optimized out>` | 编译优化级别过高 | 调试时使用 `-O0 -g` |
| core 文件没有生成 | core 限制为 0 或 systemd 接管 | `ulimit -c unlimited`，检查 `/proc/sys/kernel/core_pattern` |
| Git 冲突不知如何处理 | 两个分支修改同一位置 | 先 `git status`，手动保留正确内容，再 `git add` 和 `git commit` |

## 本讲成果

- 一份远程 GDB 调试记录，包含断点、变量和 backtrace。
- 一份 strace 或 ltrace 观察日志，能说明程序访问了哪些系统资源。
- 一个 Git 分支和至少一次实验提交，提交信息能描述变更目的。

