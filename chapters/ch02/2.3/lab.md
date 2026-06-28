# 实验 2.3 ELF 与动态库观察

## 对应讲次

| 项目 | 内容 |
| --- | --- |
| 章节 | 第二章 工具链与工程 |
| 讲次 | 2.3 |
| 课程主题 | `readelf`、`ldd` 观察二进制 |
| 实验类型 | 必做实验 |

## 实验目标

- 对 `chapters/ch02/code/hello/hello` 执行 `readelf -h` 并摘录 Machine、Class、Type。
- 执行 `readelf -l` 并指出至少一个 `LOAD` 段。
- 执行 `ldd` 或说明为何为静态链接；可选在板端复验。

## 对应知识点

| # | 知识点 | 本实验中的验证方式 |
| --- | --- | --- |
| 1 | ELF 头 | `readelf -h` |
| 2 | 程序头 | `readelf -l` |
| 3 | 架构字段 | Machine = RISC-V |
| 4 | 动态链接 | `ldd` 输出 |
| 5 | host 与 target 差异 | 可选板端 `ldd` |
| 6 | 排错 | 识别 x86-64 误编译 |

## 实验环境

| 项目 | 要求 |
| --- | --- |
| 主机环境 | 已构建 `hello` |
| 目标板 | 可选，用于板端 `ldd` |
| 软件依赖 | `readelf`、`ldd`、`file` |

## 硬件连接或部署关系

| 模块 | 引脚/接口 | 连接说明 | 注意事项 |
| --- | --- | --- | --- |
| 主机 | — | 分析 ELF | 主要工作区 |
| 目标板 | 网络 | 可选 SSH 对比 | 非必须 |

## 实验任务

### 任务 1：ELF 头与程序头

保存 `readelf -h` 全文或关键字段表；保存 `readelf -l` 前 20 行。

### 任务 2：依赖分析

运行 `ldd hello` 与 `file hello`，判断链接方式。

### 任务 3：板端对比（可选）

在板端对 `~/hello` 执行 `ldd`，与 host 结果对比。

## 实验步骤

1. 进入目录并确认文件：

```bash
cd chapters/ch02/code/hello
file hello
```

2. ELF 头：

```bash
readelf -h hello
```

3. 程序头：

```bash
readelf -l hello
```

4. 节头（浏览）：

```bash
readelf -S hello | head -n 15
```

5. 动态库：

```bash
ldd hello
```

6. 可选板端：

```bash
ssh <user>@<board-ip> 'ldd ~/hello'
```

7. 用表格归纳：Class、Machine、Type、是否动态链接、主要依赖库。

## 运行验证

| 验证项 | 预期现象 | 是否通过 |
| --- | --- | --- |
| Machine | RISC-V |  |
| LOAD 段 | `readelf -l` 中可见 |  |
| 链接方式 | `file`/`ldd` 结论明确 |  |
| 归纳表 | 字段完整 |  |

## 验收记录

| 记录项 | 内容 |
| --- | --- |
| 运行命令 | `readelf`、`ldd`、`file` |
| 关键输出 | Machine、LOAD、依赖库 |
| 截图或照片 | 终端截图 |
| 异常处理 | host 上 `ldd` not found 的解释 |

## 常见问题

| 现象 | 可能原因 | 处理方式 |
| --- | --- | --- |
| Machine 为 x86-64 | 用错编译器 | 重新 `make`，检查 `CROSS_COMPILE` |
| host `ldd` 库 not found | 交叉 sysroot 路径 | 在板端 `ldd` 或查 sysroot |
| `ldd` 非动态可执行 | 静态链接 | 在报告中说明，不强行求依赖列表 |

## 提交要求

- `readelf -h` 与 `readelf -l` 摘录（或截图）。
- `file` 与 `ldd` 输出。
- 归纳表一份。
- 可选：板端 `ldd` 对比一句结论。
