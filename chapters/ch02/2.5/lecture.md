# 2.5 统一工程目录（src/include/build）

## 本讲目标

- 能将源码、头文件与构建产物分别放入 `src/`、`include/`、`build/`。
- 能使用多文件工程 Makefile 完成编译、清理与板端部署。
- 能说明该目录结构与实验 1、综合项目的衔接关系。

## 前置条件

- 完成 2.4，理解 Makefile 变量与 `clean`。
- 完成 2.2 的 SCP 部署流程。

## 知识简介

单文件 `hello` 适合入门；真实项目通常拆分多个 `.c` 与公共头文件。统一目录约定降低后续 GPIO、MQTT、综合项目迁移成本：

```text
project-template/
├── Makefile
├── include/          # 对外头文件
├── src/              # 源文件
└── build/            # 产物（.o、可执行文件），可删可重建
```

课程模板位于 `chapters/ch02/code/project-template/`。`make` 后生成 `build/app`；`make clean` 删除整个 `build/`。

图 2-5 建议展示编译依赖：

```text
include/greet.h
src/main.c ──┐
src/greet.c ─┼──> build/src/*.o ──> build/app
Makefile ────┘
```

## 环境准备

| 项目 | 要求 | 检查方式 |
| --- | --- | --- |
| 工程目录 | `chapters/ch02/code/project-template/` | `ls src include Makefile` |
| 交叉工具链 | 与前几节一致 | `make` 无报错 |
| 板端 | SSH/SCP 可用 | 同 2.2 |

## 操作步骤

### 步骤 1：浏览工程结构

```bash
cd chapters/ch02/code/project-template
find . -type f | sort
```

阅读 `include/greet.h`、`src/main.c`、`src/greet.c`，理解声明与实现分离。

### 步骤 2：阅读 Makefile 要点

- `BUILD_DIR := build` 集中管理产物；
- `CFLAGS` 含 `-Iinclude`；
- 模式规则 `build/%.o` 将 `.c` 编译到 `build/` 下保持路径；
- `all` 依赖 `dirs` 创建目录。

### 步骤 3：构建

```bash
make clean
make
file build/app
```

### 步骤 4：部署到板端

```bash
scp build/app <user>@<board-ip>:~/
ssh <user>@<board-ip> 'chmod +x ~/app && ./app'
```

预期输出：`Hello, RISC-V Linux!`

### 步骤 5：清理并重建

```bash
make clean
test ! -d build && echo "build removed"
make
```

确认 `build/` 可安全删除且能一键重建——这是后续实验版本管理的基本习惯。

### 步骤 6：扩展练习（可选）

在 `src/` 新增 `version.c`，打印构建日期；更新 `Makefile` 的 `SRCS`。验证多文件增量编译。

## 课堂练习

为何建议把 `build/` 加入 `.gitignore`，而把 `src/` 与 `include/` 纳入版本库？若把 `.o` 文件提交到 Git 会有什么问题？

## 运行验证

| 验证项 | 预期现象 | 记录 |
| --- | --- | --- |
| `make` | 生成 `build/app` | 保存输出 |
| `file build/app` | RISC-V ELF | 保存输出 |
| 板端运行 | 正确问候输出 | 保存 SSH 输出 |
| `make clean` | `build/` 消失 | 保存 `ls` |
| 重建 | 再次 `make` 成功 | 保存输出 |

## 常见问题

### `No rule to make target 'build/src/main.o'`

未先创建 `build/src` 目录。检查 `dirs` 目标与 `@mkdir -p`。

### 头文件找不到

确认 `CFLAGS` 含 `-Iinclude`，且 `#include "greet.h"` 与文件位置一致。

### `build/app` 与 `hello` 命名

模板使用 `build/app` 区分工程名；部署时可 `scp` 后在板端重命名。

## 本讲成果

- 一套符合 `src/include/build` 约定的可运行工程。
- 板端运行 `app` 的记录。
- 后续实验可复制的 Makefile 模板路径：`chapters/ch02/code/project-template/`。
