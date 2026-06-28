# 实验 2.1 host、target、sysroot 对照

## 对应讲次

| 项目 | 内容 |
| --- | --- |
| 章节 | 第二章 工具链与工程 |
| 讲次 | 2.1 |
| 课程主题 | host、target、sysroot 概念 |
| 实验类型 | 必做实验 |

## 实验目标

- 分别确认 host 与 target 的架构和系统信息。
- 查询交叉编译器目标三元组与 sysroot 相关信息。
- 完成三要素对照表，并用自己的话说明交叉编译流程。

## 对应知识点

| # | 知识点 | 本实验中的验证方式 |
| --- | --- | --- |
| 1 | host 与 target 区分 | 对比两端 `uname -m` |
| 2 | 交叉编译定义 | 书面说明为何不能直接用主机 `gcc` 产物上板 |
| 3 | 目标三元组 | 保存 `-dumpmachine` 输出 |
| 4 | sysroot 作用 | 记录 sysroot 路径或查找方法 |
| 5 | 动态链接环境差异 | 对比两端 `file` / `ldd` 样例 |
| 6 | Linux 与裸机工具链 | 说明本课程应选哪类前缀 |

## 实验环境

| 项目 | 要求 |
| --- | --- |
| 主机环境 | 已安装 RISC-V Linux 交叉工具链 |
| 目标板 | LicheePi 4A |
| 目标系统 | 课程镜像（如 RevyOS） |
| 硬件连接 | 网络可达，SSH 可用 |
| 软件依赖 | `ssh`、`file`、`ldd`（板端） |

## 硬件连接或部署关系

| 模块 | 引脚/接口 | 连接说明 | 注意事项 |
| --- | --- | --- | --- |
| 主机 | 网络 | 执行交叉编译器查询 | 记录主机名与架构 |
| 目标板 | 网络 | SSH 登录确认 target | 与 host 架构应不同 |

## 实验任务

### 任务 1：采集 host 与 target 信息

分别记录主机名、架构、操作系统摘要。

### 任务 2：查询工具链与 sysroot

记录编译器版本、`-dumpmachine`、`-print-sysroot` 或等价信息。

### 任务 3：完成对照表并回答问题

填写表格，并回答课堂练习中的三句话判断题。

## 实验步骤

1. 在主机执行：

```bash
uname -m
hostname
riscv64-unknown-linux-gnu-gcc -dumpmachine
riscv64-unknown-linux-gnu-gcc --version | head -n 1
riscv64-unknown-linux-gnu-gcc -print-sysroot
file /bin/bash
```

2. 在板端执行：

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

4. 填写对照表（示例列：项目 / host / target / sysroot）。

5. 用三到五句话描述：从编辑 C 源码到板端运行的完整路径。

## 运行验证

| 验证项 | 预期现象 | 是否通过 |
| --- | --- | --- |
| host 架构 | 与板端不同 |  |
| target 架构 | `riscv64` |  |
| 三元组 | 含 `linux-gnu` |  |
| sysroot | 有路径或可查证的查找方式 |  |
| 对照表 | 完整无空项 |  |

## 验收记录

| 记录项 | 内容 |
| --- | --- |
| 运行命令 | 上述 host、target、工具链命令 |
| 关键输出 | 架构、三元组、sysroot、os-release |
| 截图或照片 | 终端输出截图 |
| 异常处理 | 工具链前缀不一致时的处理 |

## 常见问题

| 现象 | 可能原因 | 处理方式 |
| --- | --- | --- |
| 找不到交叉编译器 | 未安装或未加入 `PATH` | 回到 1.2 检查 `ruyi` 工具链 |
| 板端 `ldd` 报错 | 静态链接或非 ELF | 换用 `/bin/bash` 等动态程序 |
| sysroot 难定位 | 工具链打包方式不同 | 保存 `-v` 输出，查阅包文档 |

## 提交要求

- host / target / sysroot 对照表。
- 工具链 `-dumpmachine` 与版本输出。
- 三道判断题的书面答案。
- 一段交叉编译流程说明（3–5 句）。
