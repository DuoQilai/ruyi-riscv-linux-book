# 实验 2.2 Hello World 交叉编译与板端运行

## 对应讲次

| 项目 | 内容 |
| --- | --- |
| 章节 | 第二章 工具链与工程 |
| 讲次 | 2.2 |
| 课程主题 | 第一个 C 程序：编译 → 传输 → 运行 |
| 实验类型 | 必做实验 |

## 实验目标

- 使用 `chapters/ch02/code/hello/` 完成交叉编译。
- 将 `hello` 部署到 LicheePi 4A 并正确执行。
- 保存 host 与 target 两侧的 `file` 输出作为验收证据。

## 对应知识点

| # | 知识点 | 本实验中的验证方式 |
| --- | --- | --- |
| 1 | 交叉编译 | `make` 与主机侧 `file` |
| 2 | 部署 | `scp` 上传 |
| 3 | 板端验收 | `./hello` 输出 |
| 4 | 可执行权限 | `chmod +x` |
| 5 | 架构一致性 | 两端 `file` 对比 |
| 6 | 与 1.2 衔接 | 复用 SSH/SCP 通道 |

## 实验环境

| 项目 | 要求 |
| --- | --- |
| 主机环境 | Linux，已配置交叉工具链 |
| 目标板 | LicheePi 4A |
| 目标系统 | 课程镜像（如 RevyOS） |
| 硬件连接 | 网络、SSH |
| 软件依赖 | `make`、`scp`、`ssh`、`file` |
| 代码路径 | `chapters/ch02/code/hello/` |

## 硬件连接或部署关系

| 模块 | 引脚/接口 | 连接说明 | 注意事项 |
| --- | --- | --- | --- |
| 主机 | 网络 | 编译与 scp | 记录工作目录 |
| 目标板 | 网络 | 接收并运行 `hello` | 确认 IP 与用户名 |

## 实验任务

### 任务 1：构建

在工程目录执行 `make`，并检查 ELF 架构。

### 任务 2：部署

上传到板端用户家目录并设置可执行权限。

### 任务 3：运行与记录

板端执行程序，保存完整终端输出。

## 实验步骤

1. 进入工程目录：

```bash
cd chapters/ch02/code/hello
```

2. 编译：

```bash
make clean
make
file hello
```

3. 上传（替换用户名与 IP）：

```bash
scp hello <user>@<board-ip>:~/
ssh <user>@<board-ip> 'chmod +x ~/hello && file ~/hello'
```

4. 运行：

```bash
ssh <user>@<board-ip> './hello'
```

5. 可选：对比在主机执行 `./hello` 的错误信息，截图说明为何不能在 host 验证。

## 运行验证

| 验证项 | 预期现象 | 是否通过 |
| --- | --- | --- |
| `make` | 成功 |  |
| 主机 `file` | RISC-V ELF |  |
| `scp` | 无错误 |  |
| 板端 `file` | RISC-V ELF |  |
| `./hello` | 输出 `Hello, RISC-V Linux!` |  |

## 验收记录

| 记录项 | 内容 |
| --- | --- |
| 运行命令 | `make`、`file`、`scp`、`./hello` |
| 关键输出 | 两端 `file` 与程序 stdout |
| 截图或照片 | 编译与板端运行截图 |
| 异常处理 | 权限、架构或网络问题 |

## 常见问题

| 现象 | 可能原因 | 处理方式 |
| --- | --- | --- |
| `make` 找不到 gcc | `CROSS_COMPILE` 前缀错误 | 对照 1.2 工具链前缀修改 Makefile 或环境变量 |
| 板端无法执行 | 架构错误或权限 | `file`、`chmod +x` |
| 输出为空 | 连错机器 | 核对 `hostname` |

## 提交要求

- `make` 与主机侧 `file hello` 输出。
- `scp` 命令与板端 `file ~/hello` 输出。
- 板端 `./hello` 完整输出截图。
- 两三句话：为何必须在板端验收。
