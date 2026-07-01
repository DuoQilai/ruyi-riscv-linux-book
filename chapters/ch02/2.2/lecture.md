# 2.2 第一个 C 程序：编译 → 传输 → 运行

## 本讲目标

- 能使用交叉编译器构建最小 C 程序，并确认产物为 RISC-V ELF。
- 能通过 SCP 将可执行文件部署到开发板并运行。
- 能区分“在 host 编译成功”和“在 target 运行成功”两个验收环节。

## 前置条件

- 完成 2.1，已确认交叉工具链与 SSH/SCP 通道。
- 第一章 1.2 实验中的板端账号与 IP 仍可用。

## 知识简介

课程第一个完整交付闭环：编辑源码 → `make` 交叉编译 → `scp` 上传 → 板端 `chmod +x` → 执行并观察输出。示例工程位于 `chapters/ch02/code/hello/`，包含 `main.c` 与 Makefile。

主机上即使编译通过，也不能用 `./hello` 验证业务逻辑——x86_64 主机无法运行 RISC-V 二进制。正确验收必须在板端执行。

图 2-2 建议展示：

```text
main.c  ──make──>  hello (RISC-V ELF)
                         │
                    scp 上传
                         ▼
              板端 ~/hello ──./hello──>  stdout
```

## 环境准备

| 项目 | 要求 | 检查方式 |
| --- | --- | --- |
| 工程目录 | `chapters/ch02/code/hello/` | `ls main.c Makefile` |
| 交叉编译器 | Makefile 中 `CROSS_COMPILE` 正确 | `make` 无报错 |
| 板端登录 | SSH 免密或可用密码 | `ssh <user>@<board-ip> true` |
| 板端目录 | 用户家目录可写 | `ssh <user>@<board-ip> 'touch ~/write-test'` |

## 操作步骤

### 步骤 1：查看源码与 Makefile

```bash
cd chapters/ch02/code/hello
sed -n '1,20p' main.c
cat Makefile
```

`CROSS_COMPILE` 默认为 `riscv64-unknown-linux-gnu-`。若本机前缀不同，可在命令行覆盖：

```bash
make CROSS_COMPILE=<你的前缀>
```

### 步骤 2：交叉编译

```bash
make clean
make
file hello
```

`file hello` 应包含 `RISC-V` 和 `ELF`。若显示 `x86-64`，说明调用了错误的编译器。

### 步骤 3：上传到板端

将 `<user>`、`<board-ip>` 替换为实际值：

```bash
scp hello <user>@<board-ip>:~/
ssh <user>@<board-ip> 'chmod +x ~/hello && file ~/hello'
```

### 步骤 4：板端运行

```bash
ssh <user>@<board-ip> './hello'
```

预期输出：

```text
Hello, RISC-V Linux!
```

### 步骤 5：记录完整命令链

保存从 `make` 到 `./hello` 的终端输出，作为后续实验与项目的部署模板。

## 课堂练习

若 `scp` 成功但板端执行提示 `cannot execute binary file`，应依次检查哪些项目？写出至少三条。

## 运行验证

| 验证项 | 预期现象 | 记录 |
| --- | --- | --- |
| 本地 `make` | 生成 `hello` | 保存输出 |
| `file hello`（主机） | RISC-V ELF | 保存输出 |
| SCP | 上传无报错 | 记录命令 |
| `file ~/hello`（板端） | 仍为 RISC-V ELF | 保存输出 |
| 板端运行 | 打印问候字符串 | 保存输出 |

## 常见问题

### `cannot execute binary file`

常见原因：架构不匹配（用了主机 gcc）、上传损坏、或误传目录。用两端 `file` 命令核对。

### `Permission denied`

可执行位未设置。板端执行 `chmod +x ~/hello`。

### `No such file or directory`（已 chmod 仍失败）

可能是动态链接器路径与板端不一致。本示例为简单程序；若链接复杂库，需用 `ldd` 在板端检查依赖。当前 `hello` 若静态或仅依赖板载 libc，一般可直接运行。

### SCP 慢或中断

检查网络；大文件可改用 `rsync -avz`。本实验文件很小，通常一次 `scp` 即可。

## 本讲成果

- 板端可运行的 Hello World。
- 一套可复用的「编译 → scp → 运行」命令记录。
- 对应实验 2.2 的终端日志。
