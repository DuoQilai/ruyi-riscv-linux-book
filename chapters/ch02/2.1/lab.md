# 实验 2.1 交叉编译全流程：从源码到板端可执行文件

## 对应讲次

| 项目 | 内容 |
| --- | --- |
| 章节 | 第二章 工具链与工程 |
| 讲次 | 2.1 |
| 课程主题 | 交叉编译全流程：从源码到板端可执行文件 |
| 实验类型 | 必做实验 |

## 实验目标

- 分别确认 host 与 target 的架构和系统信息，完成三要素对照表。
- 查询交叉编译器目标三元组与 sysroot 信息。
- 使用 `chapters/ch02/code/hello/` 完成交叉编译，部署到 LicheePi 4A 并正确执行。
- 对构建产物执行 `readelf -h`、`readelf -l`、`ldd`，摘录关键字段并判断链接方式。

## 对应知识点

| # | 知识点 | 本实验中的验证方式 |
| --- | --- | --- |
| 1 | host 与 target 区分 | 对比两端 `uname -m` |
| 2 | 交叉编译定义 | 书面说明为何不能直接用主机 `gcc` 产物上板 |
| 3 | 目标三元组 | 保存 `-dumpmachine` 输出 |
| 4 | sysroot 作用 | 记录 sysroot 路径或查找方法 |
| 5 | 交叉编译 | `make` 与主机侧 `file` |
| 6 | 部署 | `scp` 上传 |
| 7 | 板端验收 | `./hello` 输出 |
| 8 | 架构一致性 | 两端 `file` 对比 |
| 9 | ELF 头 | `readelf -h` |
| 10 | 程序头 | `readelf -l` |
| 11 | 架构字段 | Machine = RISC-V |
| 12 | 动态链接 | `ldd` 输出 |
| 13 | 排错 | 识别 x86-64 误编译 |

## 实验环境

| 项目 | 要求 |
| --- | --- |
| 主机环境 | Linux，已安装 RISC-V Linux 交叉工具链 |
| 目标板 | LicheePi 4A |
| 目标系统 | 课程镜像（如 RevyOS） |
| 硬件连接 | 网络可达，SSH 可用 |
| 软件依赖 | `make`、`ssh`、`scp`、`file`、`readelf`、`ldd` |
| 代码路径 | `chapters/ch02/code/hello/` |

## 硬件连接或部署关系

| 模块 | 引脚/接口 | 连接说明 | 注意事项 |
| --- | --- | --- | --- |
| 主机 | 网络 | 执行交叉编译、ELF 分析与 scp | 记录主机名与架构 |
| 目标板 | 网络 | 接收并运行 `hello` | 与 host 架构应不同 |

## 实验任务

### 任务 1：采集 host 与 target 信息

分别记录主机名、架构、操作系统摘要。查询工具链 `-dumpmachine` 与 sysroot 信息。填写三要素对照表。

### 任务 2：交叉编译与部署

在 `chapters/ch02/code/hello/` 执行 `make`，检查 ELF 架构，上传到板端并运行。

### 任务 3：ELF 分析

对 `hello` 执行 `readelf -h`、`readelf -l`、`ldd`，摘录关键字段并归纳。

## 实验步骤

### 阶段 A：环境确认

1. 主机端：

```bash
uname -m
hostname
riscv64-unknown-linux-gnu-gcc -dumpmachine
riscv64-unknown-linux-gnu-gcc --version | head -n 1
riscv64-unknown-linux-gnu-gcc -print-sysroot
file /bin/bash
```

2. 板端：

```bash
uname -m
hostname
head -n 5 /etc/os-release
file /bin/bash
ldd /bin/bash | head -n 3
```

3. 若 `-print-sysroot` 为空，补充执行：

```bash
riscv64-unknown-linux-gnu-gcc -v -E - < /dev/null 2>&1 | head -n 30
```

4. 填写对照表（项目 / host / target / sysroot）。用三到五句话描述从编辑 C 源码到板端运行的完整路径。

### 阶段 B：编译与部署

5. 进入工程目录并编译：

```bash
cd chapters/ch02/code/hello
make clean
make
file hello
```

6. 上传并运行（替换用户名与 IP）：

```bash
scp hello <user>@<board-ip>:~/
ssh <user>@<board-ip> 'chmod +x ~/hello && file ~/hello'
ssh <user>@<board-ip> './hello'
```

7. 可选：在主机执行 `./hello`，截图说明为何不能在 host 验证。

### 阶段 C：ELF 分析

8. ELF 头：

```bash
readelf -h hello
```

9. 程序头：

```bash
readelf -l hello
```

10. 节头（浏览）：

```bash
readelf -S hello | head -n 15
```

11. 动态库依赖：

```bash
ldd hello
```

12. 可选板端对比：

```bash
ssh <user>@<board-ip> 'ldd ~/hello'
```

13. 用表格归纳：Class、Machine、Type、是否动态链接、主要依赖库。

## 运行验证

| 验证项 | 预期现象 | 是否通过 |
| --- | --- | --- |
| host 架构 | 与板端不同 |  |
| target 架构 | `riscv64` |  |
| 三元组 | 含 `linux-gnu` |  |
| sysroot | 有路径或可查证的查找方式 |  |
| 对照表 | 完整无空项 |  |
| `make` | 成功 |  |
| 主机 `file` | RISC-V ELF |  |
| `scp` | 无错误 |  |
| 板端 `file` | RISC-V ELF |  |
| `./hello` | 输出 `Hello, RISC-V Linux!` |  |
| Machine | RISC-V |  |
| LOAD 段 | `readelf -l` 中可见 |  |
| 链接方式 | `file`/`ldd` 结论明确 |  |
| 归纳表 | 字段完整 |  |

## 验收记录

| 记录项 | 内容 |
| --- | --- |
| 运行命令 | host/target 信息采集、`make`、`file`、`scp`、`./hello`、`readelf`、`ldd` |
| 关键输出 | 架构、三元组、sysroot、两端 `file`、程序 stdout、ELF 关键字段 |
| 截图或照片 | 编译、板端运行、ELF 分析截图 |
| 异常处理 | 工具链前缀不一致、权限、架构或网络问题 |

## 常见问题

| 现象 | 可能原因 | 处理方式 |
| --- | --- | --- |
| 找不到交叉编译器 | 未安装或未加入 `PATH` | 回到第一章检查 `ruyi` 工具链 |
| Machine 为 x86-64 | 用错编译器 | 重新 `make`，检查 `CROSS_COMPILE` |
| `make` 找不到 gcc | `CROSS_COMPILE` 前缀错误 | 对照工具链前缀修改 Makefile 或环境变量 |
| 板端无法执行 | 架构错误或权限 | `file`、`chmod +x` |
| 输出为空 | 连错机器 | 核对 `hostname` |
| host `ldd` 库 not found | 交叉 sysroot 路径 | 在板端 `ldd` 或查 sysroot |
| `ldd` 非动态可执行 | 静态链接 | 在报告中说明，不强行求依赖列表 |
| 板端 `ldd` 报错 | 静态链接或非 ELF | 换用 `/bin/bash` 等动态程序 |
| sysroot 难定位 | 工具链打包方式不同 | 保存 `-v` 输出，查阅包文档 |

## 提交要求

- host / target / sysroot 对照表。
- 工具链 `-dumpmachine` 与版本输出。
- 三道课堂判断题的书面答案。
- 一段交叉编译流程说明（3–5 句）。
- `make` 与主机侧 `file hello` 输出。
- `scp` 命令与板端 `file ~/hello` 输出。
- 板端 `./hello` 完整输出截图。
- 两三句话：为何必须在板端验收。
- `readelf -h` 与 `readelf -l` 摘录（或截图）。
- `file` 与 `ldd` 输出。
- ELF 归纳表一份。
- 可选：板端 `ldd` 对比一句结论。
