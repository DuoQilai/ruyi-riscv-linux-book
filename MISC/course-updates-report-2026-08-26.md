# 课程内容更新汇报（第 1–4 章）

- **汇报范围**：近 10 天（2026-08-20 ～ 08-26，共 15 个提交）对第 1–4 章课件与实验的更新
- **在线预览**：[本课程 GitHub Pages](https://duoqilai.github.io/ruyi-riscv-linux-book/) ｜ [ruyiSDK 官方文档](https://ruyisdk.org/)

## 总体说明

本轮按评审意见逐条落实：**以实验产出为章节题目、每个知识点配最小化实例、注重知识递进（从浅到深，节节高）**。具体做法：

- 各章开头新增**术语表**（术语｜是什么｜为什么需要它），先讲清概念再往下推进；
- 为每个知识点补充**真实测过的引导实例**（含实机运行输出与反汇编对比）；
- 新增**完整可运行的示例程序**（源码 + Makefile，板上验证）。

## 各章更新明细

### 第 1 章

**ruyiSDK 相关（本部分放最前）**

- **ruyiSDK 完整安装流程**：PyPI 预编译二进制安装、PATH 检查与版本验证（`command -v ruyi`、`ruyi --version`）、`ruyi update` + `ruyi install gnu-ruyisdk`、`ruyi venv` 创建虚拟环境（`venv-gnu-ruyisdk`）并 `ruyi-activate`、交叉编译器验证（`riscv64-ruyisdk-linux-gnu-gcc -dumpmachine`）。
- **`ruyi device provision` 一键烧录（主路径）**：交互式下载并写入 RevyOS 镜像（底层走 fastboot，包索引 `revyos-sipeed-lpi4a` + `uboot-revyos-sipeed-lpi4a-16g`），并附 ruyiSDK Support Matrix（LicheePi 4A / RevyOS）链接。
- **最小 RISC-V 可执行文件实证**：交叉编译 mini 程序，`file` 识别架构，实机演示跨架构执行被内核拒绝（`Exec format error`，exit=126）。

**其他新增**

- 术语表（16 词）：指令集、x86_64 / riscv64、RISC-V、RevyOS、交叉编译、工具链、三元组、ELF、镜像、烧录、UART/串口、SSH、SCP、虚拟环境、sysroot 等。
- 引导实例：三元组逐字段拆解（CPU-厂商-系统）、同源码双编译器 + `objdump -d` 反汇编对比、ELF `e_machine` 字段讲解、串口 / SSH / SCP 三通道对比表。

### 第 2 章

**其他新增**

- 术语表（19 词）：类型、变量/常量、控制流、滞回、函数、声明/定义、头文件、编译/链接、`_start`/`main`、指针、数组、结构体、生命周期/存储期、栈/数据段、字符串、位运算、函数指针、宏/条件编译、设计化初始化。
- 引导实例（各知识点逐一补，浅色卡片标注）：整数除法截断与类型转换（温度占比）、`static` 文件内可见性（sensor.c）、传值 vs 传指针（bump / bump2，对应实验 `read_temp(&t)`）、数组名即首元素地址（`sizeof` 陷阱）。

### 第 3 章

**ruyiSDK 相关（本部分放最前）**

- **统一交叉编译工作流**：主路径在 x86 Linux 上激活 `venv-gnu-ruyisdk` 交叉编译后拷板运行；同一 Makefile 在板上自动改用板载 gcc（`CROSS_COMPILE=riscv64-ruyisdk-linux-gnu-`），主机缺 libgpiod 时可直接板上 `make`。

**其他新增**

- **寄存器 / 位心智模型**：外设 = 寄存器、控制 = 改位，把第二章位运算落到硬件（gpioset、gpiod、devmem 同一件事、入口不同）。
- **本板实测编号**：gpiochip5、line 5 = 内核全局号 gpio-651、数据寄存器物理地址 `0xffec006000`，编程用 gpiochip5 + line 5。
- **devmem 被拒实证**：内核默认 `CONFIG_STRICT_DEVMEM` 下 devmem mmap 报 `Operation not permitted`，改用内核视图 `/sys/kernel/debug/gpio` 核对电平。
- **gpioset 实测**：控 IO1_5 看内核视角、`gpioset` 为持线命令（进程活着一直占线）、`gpioget` 占用冲突演示、用 pkill/Ctrl+C 释放。
- **完整示例 relay-toggle.c**：gpiod API 生命周期（open chip → 配置并申请 line 5 → 拉高 → 拉低 → 释放），板上验证；与实验 `temperature_fan`（3 函数结构 + 滞回决策）区分定位。
- **面包板与电烙铁**：面包板电源轨/连接孔/隔离凹槽连通规则、接线安全提示、电烙铁（约 40W）与焊锡使用（新增图 4、图 5）。

### 第 4 章

**ruyiSDK 相关（本部分放最前）**

- **统一交叉编译工作流**：与第三章一致，激活 `venv-gnu-ruyisdk` 后交叉编译拷板运行，Makefile 按机器自动选编译器。

**其他新增**

- 术语表（15 词）：串口/UART、stdin、stdout、fgets、传感器、单总线、电平转换/TXS、命令表、切词、派发 dispatch、select、超时、阈值、滞回、模拟模式。
- **select 模型**：让内核同时盯多个来源（该采样了 / 你打字了）+ 超时机制讲解。
- **cmd-demo 完整命令读取器**：先分再合——处理函数 → 命令表（`cmd_entry` 结构体 + 函数指针）→ 合成完整程序；help/echo/quit 三命令骨架，实验的 status/set 同构自写。
- 真实对话片段示例。

## 汇总表

| 章节 | ruyiSDK 相关新增 | 其他知识点新增 | 示例/实例 |
|---|---|---|---|
| 第 1 章 | ruyiSDK 安装、`ruyi venv` 虚拟环境、`ruyi device provision` 一键烧录、交叉编译器验证 | 术语表 16 词；三元组拆解、反汇编对比、ELF `e_machine`、三通道对比 | mini 程序交叉编译 + 实机运行（exit=0 / 126） |
| 第 2 章 | — | 术语表 19 词；类型转换、static、传值/传指针、数组与 sizeof | 各知识点浅色引导实例 |
| 第 3 章 | 交叉编译工作流（激活 venv → make → 拷板运行） | 寄存器/位心智模型、本板 GPIO 实测编号、devmem 被拒实证、gpioset 持线、面包板与电烙铁 | relay-toggle.c 完整 gpiod 生命周期（板上验证） |
| 第 4 章 | 交叉编译工作流（同第三章） | 术语表 15 词；select 模型、命令表/切词/派发 | cmd-demo 完整命令读取器（先分再合） |

## 其他维护性改动（非知识点）

- **用词标准化**（ch01–04）：非正式表达改为规范术语（聊天线→串口、能跑→可运行、狂开狂关→频繁开合），并改写口语化表述（如「把风扇饿死」改为「一直阻塞在打字上顾不上采样」）。
- **课件结构调整**：ch03 拆为 4 节重排、移除 consumer-naming 提示框；示例更名 fan-toggle → relay-toggle（与实验 `temperature_fan` 区分）。
- **链接与发布**：修正 GitHub Pages URL、补充在线预览地址与发布分支说明（`main` 自动发布）。
