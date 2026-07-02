# 2.2 Makefile 与工程目录规范

## 本讲目标

- 能编写包含目标、依赖、命令的基本 Makefile 规则，并理解 Tab 缩进要求。
- 能使用变量定义 `CC`、`CFLAGS`、`CROSS_COMPILE` 等，避免硬编码重复。
- 能实现 `make`、`make clean` 及 `.PHONY` 伪目标，保持构建目录可重复生成。
- 能将源码、头文件与构建产物分别放入 `src/`、`include/`、`build/`。
- 能使用多文件工程 Makefile 完成编译、清理与板端部署。
- 能说明该目录结构与后续实验、综合项目的衔接关系。

## 前置条件

- 完成 2.1，理解交叉编译与 SCP 部署流程。
- 熟悉命令行 `make` 的基本用法。

## 知识简介

### Makefile 基础

Makefile 描述"什么文件由什么文件生成、执行什么命令"。`make` 根据文件时间戳决定是否需要重新编译。课程工程将统一通过 Makefile 入口构建，便于后续实验和综合项目扩展。

`chapters/ch02/code/hello/Makefile` 是单文件示例；`chapters/ch02/code/project-template/Makefile` 在此基础上增加多源文件与 `build/` 目录。

图 2-4：Makefile 工作机制

```text
Makefile
  CROSS_COMPILE, CC, CFLAGS
        ↓
  hello: main.c
      $(CC) $(CFLAGS) -o $@ $^
        ↓
  make / make clean
```

### 统一工程目录

单文件 `hello` 适合入门；真实项目通常拆分多个 `.c` 与公共头文件。统一目录约定降低后续 GPIO、MQTT、综合项目迁移成本：

```text
project-template/
├── Makefile
├── include/          # 对外头文件
├── src/              # 源文件
└── build/            # 产物（.o、可执行文件），可删可重建
```

课程模板位于 `chapters/ch02/code/project-template/`。`make` 后生成 `build/app`；`make clean` 删除整个 `build/`。

图 2-5：多文件编译依赖

```text
include/greet.h
src/main.c ──┐
src/greet.c ─┼──> build/src/*.o ──> build/app
Makefile ────┘
```

## 环境准备

| 项目 | 要求 | 检查方式 |
| --- | --- | --- |
| 单文件工程 | `chapters/ch02/code/hello/` | 存在 `Makefile` |
| 多文件工程 | `chapters/ch02/code/project-template/` | `ls src include Makefile` |
| make | GNU Make | `make --version` |
| 交叉工具链 | 与 2.1 一致 | `make` 成功 |
| 板端 | SSH/SCP 可用 | 同 2.1 |

## 操作步骤

### 步骤 1：阅读单文件 Makefile

```bash
cd chapters/ch02/code/hello
cat -A Makefile   # 可见行尾与 Tab（^I）
```

确认：`CC` 由 `CROSS_COMPILE` 派生；编译规则使用 `$@`（目标）与 `$^`（依赖）；`clean` 删除产物。

### 步骤 2：体验增量构建

```bash
make clean
make
make          # 第二次应提示已是最新
touch main.c
make          # 应重新编译
```

理解"依赖变化 → 重新构建"的行为。

### 步骤 3：添加 `CFLAGS` 演示

在 Makefile 中确保有 `-Wall -Wextra`。故意写一处未使用变量，观察警告：

```c
int main(void) {
    int unused = 0;
    printf("Hello, RISC-V Linux!\n");
    return 0;
}
```

执行 `make`，应出现 unused 警告；修复后警告消失。实验结束恢复原代码。

### 步骤 4：命令行覆盖变量

不修改文件的情况下试用不同优化级别：

```bash
make clean
make CFLAGS='-Wall -O0 -g'
```

说明 Makefile 变量可被命令行覆盖，便于临时调试。

### 步骤 5：实现并验证 `clean`

```bash
make clean
ls hello 2>&1
make
```

`clean` 后 `hello` 应不存在；再次 `make` 可恢复。

### 步骤 6：浏览多文件工程结构

```bash
cd chapters/ch02/code/project-template
find . -type f | sort
```

阅读 `include/greet.h`、`src/main.c`、`src/greet.c`，理解声明与实现分离。

### 步骤 7：阅读多文件 Makefile 要点

- `BUILD_DIR := build` 集中管理产物；
- `CFLAGS` 含 `-Iinclude`；
- 模式规则 `build/%.o` 将 `.c` 编译到 `build/` 下保持路径；
- `all` 依赖 `dirs` 创建目录。

### 步骤 8：构建多文件工程

```bash
make clean
make
file build/app
```

### 步骤 9：部署到板端

```bash
scp build/app <user>@<board-ip>:~/
ssh <user>@<board-ip> 'chmod +x ~/app && ./app'
```

预期输出：`Hello, RISC-V Linux!`

### 步骤 10：清理并重建

```bash
make clean
test ! -d build && echo "build removed"
make
```

确认 `build/` 可安全删除且能一键重建——这是后续实验版本管理的基本习惯。

### 步骤 11：扩展练习（可选）

在 `src/` 新增 `version.c`，打印构建日期；更新 `Makefile` 的 `SRCS`。验证多文件增量编译。

## 课堂练习

1. 解释下列 Makefile 错误为何会导致 `make` 失败或行为异常：
   - 命令行前使用了空格而非 Tab。
   - 目标 `hello` 未声明依赖 `main.c`，且 `hello` 文件已存在但未更新。
   - `clean` 目标未标记 `.PHONY`，且目录下恰好有名为 `clean` 的文件。
2. 为何建议把 `build/` 加入 `.gitignore`，而把 `src/` 与 `include/` 纳入版本库？若把 `.o` 文件提交到 Git 会有什么问题？

## 运行验证

| 验证项 | 预期现象 | 记录 |
| --- | --- | --- |
| 首次 `make` | 生成 `hello` | 保存输出 |
| 无改动再 `make` | 提示 nothing to be done | 保存输出 |
| `touch main.c` | 触发重编 | 保存输出 |
| `make clean` | 删除 `hello` | 保存 `ls` 结果 |
| 警告实验 | `-Wall` 能捕获问题 | 可选截图 |
| 多文件 `make` | 生成 `build/app` | 保存输出 |
| `file build/app` | RISC-V ELF | 保存输出 |
| 板端运行 | 正确问候输出 | 保存 SSH 输出 |
| `make clean`（多文件） | `build/` 消失 | 保存 `ls` |
| 重建 | 再次 `make` 成功 | 保存输出 |

## 常见问题

### `missing separator`

命令行必须以 Tab 开头。用 `cat -A` 检查，不要用空格替代。

### `make: *** No rule to make target`

检查依赖文件名、路径与 `SRC` 变量是否一致。

### `clean` 不删除文件

检查 `rm` 目标名是否与产物一致；`project-template` 中应 `rm -rf build`。

### `No rule to make target 'build/src/main.o'`

未先创建 `build/src` 目录。检查 `dirs` 目标与 `@mkdir -p`。

### 头文件找不到

确认 `CFLAGS` 含 `-Iinclude`，且 `#include "greet.h"` 与文件位置一致。

### `build/app` 与 `hello` 命名

模板使用 `build/app` 区分工程名；部署时可 `scp` 后在板端重命名。

### 始终重编

依赖未写或时间戳异常。检查 `hello: main.c` 依赖行。

## 本讲成果

- 能解释 `hello` Makefile 各变量与规则含义。
- 完成 `make` / `make clean` / 增量构建演示记录。
- 一套符合 `src/include/build` 约定的可运行工程。
- 板端运行 `app` 的记录。
- 后续实验可复制的 Makefile 模板路径：`chapters/ch02/code/project-template/`。
- 对应实验 2.2 的终端输出与验收材料。
