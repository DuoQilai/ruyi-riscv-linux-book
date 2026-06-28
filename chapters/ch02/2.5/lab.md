# 实验 2.5 统一工程目录模板

## 对应讲次

| 项目 | 内容 |
| --- | --- |
| 章节 | 第二章 工具链与工程 |
| 讲次 | 2.5 |
| 课程主题 | 统一工程目录（`src`/`include`/`build`） |
| 实验类型 | 必做实验 |

## 实验目标

- 基于 `chapters/ch02/code/project-template/` 完成多文件交叉编译。
- 将 `build/app` 部署到板端并运行。
- 验证 `make clean` 可删除 `build/` 且能完整重建。

## 对应知识点

| # | 知识点 | 本实验中的验证方式 |
| --- | --- | --- |
| 1 | 目录约定 | 提交 `find` 树状列表 |
| 2 | 头文件路径 | `-Iinclude` 编译通过 |
| 3 | 多源文件链接 | `main.c` + `greet.c` |
| 4 | 产物隔离 | 仅 `build/` 含 `.o` 与二进制 |
| 5 | 部署 | `scp` + 板端运行 |
| 6 | 工程模板 | 说明如何复用到实验 1 |

## 实验环境

| 项目 | 要求 |
| --- | --- |
| 代码路径 | `chapters/ch02/code/project-template/` |
| 目标板 | LicheePi 4A |
| 目标系统 | 课程镜像（如 RevyOS） |
| 软件依赖 | `make`、`scp`、`ssh` |

## 硬件连接或部署关系

| 模块 | 引脚/接口 | 连接说明 | 注意事项 |
| --- | --- | --- | --- |
| 主机 | 网络 | 构建与上传 | 工作目录为 project-template |
| 目标板 | 网络 | 运行 `app` | 同 2.2 |

## 实验任务

### 任务 1：构建多文件工程

`make clean && make`，检查 `build/app`。

### 任务 2：板端部署运行

上传并执行，保存输出。

### 任务 3：清理与重建

证明 `build/` 可删且可重建。

## 实验步骤

1. 查看结构：

```bash
cd chapters/ch02/code/project-template
find . -type f | sort
```

2. 构建：

```bash
make clean
make
file build/app
```

3. 部署：

```bash
scp build/app <user>@<board-ip>:~/
ssh <user>@<board-ip> 'chmod +x ~/app && ./app'
```

4. 清理重建：

```bash
make clean
ls build 2>&1
make
```

5. 在报告中用示意图或树状列表说明 `src/`、`include/`、`build/` 各自职责。

## 运行验证

| 验证项 | 预期现象 | 是否通过 |
| --- | --- | --- |
| `make` | 成功 |  |
| `build/app` | RISC-V ELF |  |
| 板端 `./app` | `Hello, RISC-V Linux!` |  |
| `make clean` | 无 `build/` |  |
| 重建 | 再次成功 |  |

## 验收记录

| 记录项 | 内容 |
| --- | --- |
| 运行命令 | `make`、`scp`、板端执行 |
| 关键输出 | `file`、程序 stdout |
| 截图或照片 | 目录树与运行截图 |
| 异常处理 | 头文件或 mkdir 问题 |

## 常见问题

| 现象 | 可能原因 | 处理方式 |
| --- | --- | --- |
| 找不到 greet.h | 缺少 `-Iinclude` | 检查 Makefile `CFLAGS` |
| 链接错误 | `SRCS` 漏文件 | 补全 `src/greet.c` |
| 板端无法运行 | 架构或权限 | 同实验 2.2 排查 |

## 提交要求

- 工程目录树（`find` 输出或等价图）。
- `make` 与 `file build/app` 输出。
- 板端运行截图。
- `make clean` 前后对比。
- 两三句话：实验 1 如何基于此模板扩展。
