# 2.4 Makefile：规则、变量、clean

## 本讲目标

- 能编写包含目标、依赖、命令的基本 Makefile 规则，并理解 Tab 缩进要求。
- 能使用变量定义 `CC`、`CFLAGS`、`CROSS_COMPILE` 等，避免硬编码重复。
- 能实现 `make`、`make clean` 及 `.PHONY` 伪目标，保持构建目录可重复生成。

## 前置条件

- 完成 2.2，已能手动编译 `hello`。
- 熟悉命令行 `make` 的基本用法。

## 知识简介

Makefile 描述“什么文件由什么文件生成、执行什么命令”。`make` 根据文件时间戳决定是否需要重新编译。课程工程将统一通过 Makefile 入口构建，便于实验 1、综合项目扩展。

`chapters/ch02/code/hello/Makefile` 是单文件示例；`chapters/ch02/code/project-template/Makefile` 在此基础上增加多源文件与 `build/` 目录，本讲先掌握前者，下一讲迁移后者。

图 2-4 建议展示：

```text
Makefile
  CROSS_COMPILE, CC, CFLAGS
        ↓
  hello: main.c
      $(CC) $(CFLAGS) -o $@ $^
        ↓
  make / make clean
```

## 环境准备

| 项目 | 要求 | 检查方式 |
| --- | --- | --- |
| 工程 | `chapters/ch02/code/hello/` | 存在 `Makefile` |
| make | GNU Make | `make --version` |
| 工具链 | 与 2.2 一致 | `make` 成功 |

## 操作步骤

### 步骤 1：阅读现有 Makefile

```bash
cd chapters/ch02/code/hello
cat -A Makefile   # 可见行尾与 Tab（^I）
```

确认：

- `CC` 由 `CROSS_COMPILE` 派生；
- 编译规则使用 `$@`（目标）与 `$^`（依赖）；
- `clean` 删除产物。

### 步骤 2：增量构建体验

```bash
make clean
make
make          # 第二次应提示已是最新
touch main.c
make          # 应重新编译
```

理解“依赖变化 → 重新构建”的行为。

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

## 课堂练习

解释下列 Makefile 错误为何会导致 `make` 失败或行为异常：

1. 命令行前使用了空格而非 Tab。
2. 目标 `hello` 未声明依赖 `main.c`，且 `hello` 文件已存在但未更新。
3. `clean` 目标未标记 `.PHONY`，且目录下恰好有名为 `clean` 的文件。

## 运行验证

| 验证项 | 预期现象 | 记录 |
| --- | --- | --- |
| 首次 `make` | 生成 `hello` | 保存输出 |
| 无改动再 `make` | 提示 nothing to be done | 保存输出 |
| `touch main.c` | 触发重编 | 保存输出 |
| `make clean` | 删除 `hello` | 保存 `ls` 结果 |
| 警告实验 | `-Wall` 能捕获问题 | 可选截图 |

## 常见问题

### `missing separator`

命令行必须以 Tab 开头。用 `cat -A` 检查，不要用空格替代。

### `make: *** No rule to make target`

检查依赖文件名、路径与 `SRC` 变量是否一致。

### `clean` 不删除文件

检查 `rm` 目标名是否与产物一致；`project-template` 中应 `rm -rf build`。

## 本讲成果

- 能解释 `hello` Makefile 各变量与规则含义。
- 完成 `make` / `make clean` / 增量构建演示记录。
- 对应实验 2.4 的 Makefile 与终端输出。
