# 实验 2.2 Makefile 与工程目录规范

## 对应讲次

| 项目 | 内容 |
| --- | --- |
| 章节 | 第二章 工具链与工程 |
| 讲次 | 2.2 |
| 课程主题 | Makefile 与工程目录规范 |
| 实验类型 | 必做实验 |

## 实验目标

- 读懂并必要时微调 `chapters/ch02/code/hello/Makefile`，演示增量构建与 `make clean`。
- 通过 `-Wall` 体验编译器警告与修复流程。
- 基于 `chapters/ch02/code/project-template/` 完成多文件交叉编译，部署到板端并运行。
- 验证 `make clean` 可删除 `build/` 且能完整重建。

## 对应知识点

| # | 知识点 | 本实验中的验证方式 |
| --- | --- | --- |
| 1 | 目标与依赖 | 修改 `main.c` 触发重编 |
| 2 | 变量 | 解释 `CC`、`CFLAGS`、`CROSS_COMPILE` |
| 3 | 自动变量 | 说明 `$@`、`$^` |
| 4 | `.PHONY` | 说明 `clean` 为何是伪目标 |
| 5 | `make clean` | 产物被删除 |
| 6 | 命令行覆盖 | `make CFLAGS=...` |
| 7 | 目录约定 | 提交 `find` 树状列表 |
| 8 | 头文件路径 | `-Iinclude` 编译通过 |
| 9 | 多源文件链接 | `main.c` + `greet.c` |
| 10 | 产物隔离 | 仅 `build/` 含 `.o` 与二进制 |
| 11 | 部署 | `scp` + 板端运行 |
| 12 | 工程模板 | 说明如何复用到后续实验 |

## 实验环境

| 项目 | 要求 |
| --- | --- |
| 单文件工程 | `chapters/ch02/code/hello/` |
| 多文件工程 | `chapters/ch02/code/project-template/` |
| 目标板 | LicheePi 4A |
| 目标系统 | 课程镜像（如 RevyOS） |
| 软件依赖 | GNU `make`、交叉 `gcc`、`scp`、`ssh` |

## 硬件连接或部署关系

| 模块 | 引脚/接口 | 连接说明 | 注意事项 |
| --- | --- | --- | --- |
| 主机 | — | 构建验证 | 单文件实验仅在主机完成 |
| 主机 | 网络 | 多文件工程上传 | 工作目录为 project-template |
| 目标板 | 网络 | 运行 `app` | 同实验 2.1 |

## 实验任务

### 任务 1：单文件 Makefile 构建与增量编译

记录连续两次 `make` 的输出差异；演示 `clean` 与重建；通过临时引入未使用变量体验 `-Wall` 警告。

### 任务 2：多文件工程构建与部署

基于 `project-template/` 完成 `make`，将 `build/app` 部署到板端并运行。

### 任务 3：清理与重建验证

证明 `build/` 可删且可重建。

## 实验步骤

### 阶段 A：单文件 Makefile

1. 阅读 Makefile：

```bash
cd chapters/ch02/code/hello
cat Makefile
```

2. 完整构建流程：

```bash
make clean
make
make
touch main.c
make
```

3. 清理：

```bash
make clean
ls hello 2>&1
make
```

4. 警告实验（修改 `main.c` 引入未使用变量，截图警告，然后恢复源码）：

```bash
make clean && make
```

5. 变量覆盖：

```bash
make clean
make CFLAGS='-Wall -Wextra -O0 -g'
file hello
```

6. 在实验报告中用三五句话解释：规则 `hello: main.c` 中三行的含义。

### 阶段 B：多文件工程

7. 查看结构：

```bash
cd chapters/ch02/code/project-template
find . -type f | sort
```

8. 构建：

```bash
make clean
make
file build/app
```

9. 部署：

```bash
scp build/app <user>@<board-ip>:~/
ssh <user>@<board-ip> 'chmod +x ~/app && ./app'
```

10. 清理重建：

```bash
make clean
ls build 2>&1
make
```

11. 在报告中用示意图或树状列表说明 `src/`、`include/`、`build/` 各自职责。

## 运行验证

| 验证项 | 预期现象 | 是否通过 |
| --- | --- | --- |
| 首次 build | 成功 |  |
| 二次 `make` | 无重编或提示 up to date |  |
| `touch` 后 | 重编 |  |
| `clean` | `hello` 不存在 |  |
| 警告 | 能触发并消除 |  |
| 多文件 `make` | 成功 |  |
| `build/app` | RISC-V ELF |  |
| 板端 `./app` | `Hello, RISC-V Linux!` |  |
| `make clean` | 无 `build/` |  |
| 重建 | 再次成功 |  |

## 验收记录

| 记录项 | 内容 |
| --- | --- |
| 运行命令 | `make`、`make clean`、`CFLAGS` 覆盖、`scp`、板端执行 |
| 关键输出 | 增量构建与 clean 前后 `ls`、`file`、程序 stdout |
| 截图或照片 | 警告与修复（可选）、目录树与运行截图 |
| 异常处理 | Tab/路径问题、头文件或 mkdir 问题 |

## 常见问题

| 现象 | 可能原因 | 处理方式 |
| --- | --- | --- |
| missing separator | 空格缩进 | 改 Tab |
| clean 无效 | 路径或变量错误 | 对照 Makefile `rm` 行 |
| 始终重编 | 依赖未写或时间戳异常 | 检查 `hello: main.c` |
| 找不到 greet.h | 缺少 `-Iinclude` | 检查 Makefile `CFLAGS` |
| 链接错误 | `SRCS` 漏文件 | 补全 `src/greet.c` |
| 板端无法运行 | 架构或权限 | 同实验 2.1 排查 |

## 提交要求

- 当前使用的 `hello/Makefile` 全文（若未改则直接提交仓库版本）。
- `make`、`make clean`、增量构建终端记录。
- 对 `CC`/`CFLAGS`/`CROSS_COMPILE` 各一句说明。
- 三道课堂练习题的简短答案。
- 工程目录树（`find` 输出或等价图）。
- `make` 与 `file build/app` 输出。
- 板端运行截图。
- `make clean` 前后对比。
- 两三句话：后续实验如何基于此模板扩展。
