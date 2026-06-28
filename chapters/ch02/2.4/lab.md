# 实验 2.4 Makefile 构建与清理

## 对应讲次

| 项目 | 内容 |
| --- | --- |
| 章节 | 第二章 工具链与工程 |
| 讲次 | 2.4 |
| 课程主题 | Makefile：规则、变量、`clean` |
| 实验类型 | 必做实验 |

## 实验目标

- 读懂并必要时微调 `chapters/ch02/code/hello/Makefile`。
- 演示增量构建与 `make clean` 行为。
- 通过 `-Wall` 体验编译器警告与修复流程。

## 对应知识点

| # | 知识点 | 本实验中的验证方式 |
| --- | --- | --- |
| 1 | 目标与依赖 | 修改 `main.c` 触发重编 |
| 2 | 变量 | 解释 `CC`、`CFLAGS`、`CROSS_COMPILE` |
| 3 | 自动变量 | 说明 `$@`、`$^` |
| 4 | `.PHONY` | 说明 `clean` 为何是伪目标 |
| 5 | `make clean` | 产物被删除 |
| 6 | 命令行覆盖 | `make CFLAGS=...` |

## 实验环境

| 项目 | 要求 |
| --- | --- |
| 代码路径 | `chapters/ch02/code/hello/` |
| 软件依赖 | GNU `make`、交叉 `gcc` |

## 硬件连接或部署关系

本实验仅在主机完成构建验证；可选将最终 `hello` 上传板端，非必须。

## 实验任务

### 任务 1：构建与增量编译

记录连续两次 `make` 的输出差异。

### 任务 2：清理与重建

执行 `make clean` 后确认产物消失，再 `make` 恢复。

### 任务 3：警告实验

临时引入未使用变量，观察 `-Wall` 警告并修复。

## 实验步骤

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

4. 警告实验（修改 `main.c` 后编译，截图警告，然后恢复源码）：

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

## 运行验证

| 验证项 | 预期现象 | 是否通过 |
| --- | --- | --- |
| 首次 build | 成功 |  |
| 二次 `make` | 无重编或提示 up to date |  |
| `touch` 后 | 重编 |  |
| `clean` | `hello` 不存在 |  |
| 警告 | 能触发并消除 |  |

## 验收记录

| 记录项 | 内容 |
| --- | --- |
| 运行命令 | `make`、`make clean`、`CFLAGS` 覆盖 |
| 关键输出 | 增量构建与 clean 前后 `ls` |
| 截图或照片 | 警告与修复（可选） |
| 异常处理 | Tab/路径问题 |

## 常见问题

| 现象 | 可能原因 | 处理方式 |
| --- | --- | --- |
| missing separator | 空格缩进 | 改 Tab |
| clean 无效 | 路径或变量错误 | 对照 Makefile `rm` 行 |
| 始终重编 | 依赖未写或时间戳异常 | 检查 `hello: main.c` |

## 提交要求

- 当前使用的 `Makefile` 全文（若未改则直接提交仓库版本）。
- `make`、`make clean`、增量构建终端记录。
- 对 `CC`/`CFLAGS`/`CROSS_COMPILE` 各一句说明。
- 三道课堂练习题的简短答案。
