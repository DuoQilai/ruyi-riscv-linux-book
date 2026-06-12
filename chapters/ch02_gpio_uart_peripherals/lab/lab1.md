# 实验 2.1：4 路 LED 流水灯

## 对应讲次

| 项目 | 内容 |
| --- | --- |
| 章节 | 第 2 章 GPIO、UART、常用外设 |
| 讲次 | 第 1 讲 |
| 课程主题 | 面包板基础与 GPIO 输出 |
| 实验类型 | 必做实验 |

大纲讲次原文：第1讲 面包板基础与 GPIO 输出。
大纲实验原文：面包板搭建 4 路 LED 流水灯电路，编写 gpiod 控制程序实现多种点亮模式
大纲知识点原文：面包板接线规范；电路安全基础；Linux GPIO 子系统；字符设备方式控制 GPIO；LED 闪烁（Blink）；多路 LED 流水灯。

## 实验目标

- 搭建 4 路 LED 面包板电路，完成限流电阻、极性和共地检查。
- 使用 `gpioinfo` 记录 LicheePi 4A 的 GPIO chip/line 信息。
- 使用 gpiod 命令点亮单路 LED。
- 编写 C 程序实现轮流、来回、全亮全灭三种流水灯模式。

## 对应知识点

| # | 知识点 | 本实验中的验证方式 |
| --- | --- | --- |
| 1 | 面包板接线规范 | 使用万用表确认电源轨和 LED 支路通断 |
| 2 | 电路安全基础 | 每路 LED 串联限流电阻，记录阻值和极性 |
| 3 | Linux GPIO 子系统 | 使用 `gpioinfo` 查询 GPIO line |
| 4 | 字符设备方式控制 GPIO | 使用 `gpioset` 和 libgpiod C API 控制输出 |
| 5 | LED 闪烁 | 单路 LED 周期闪烁 |
| 6 | 多路 LED 流水灯 | 4 路 LED 按数组和模式表输出 |

## 实验环境

| 项目 | 要求 |
| --- | --- |
| 主机环境 | 已安装 RISC-V 交叉编译器、`make`、`ssh`、`scp` |
| 目标板 | LicheePi 4A |
| 目标系统 | RevyOS |
| 硬件连接 | 面包板、LED x4、220 欧姆到 1 千欧姆限流电阻 x4、杜邦线 |
| 软件依赖 | 板端 `gpiod`、`libgpiod-dev`，主机工程模板 |

## 硬件连接或部署关系

| 模块 | 引脚/接口 | 连接说明 | 注意事项 |
| --- | --- | --- | --- |
| LED1-LED4 | 4 个 GPIO line | GPIO -> 限流电阻 -> LED 正极，LED 负极接 GND | GPIO line 以实测映射为准 |
| GND | LicheePi 4A GND | 与面包板地线相连 | 所有 LED 共地 |
| 主机 | SSH/SCP | 部署编译后的程序 | 先确认网络连通 |

## 实验任务

### 任务 1：完成接线检查

断电状态下完成 4 路 LED 接线，使用万用表确认每路 LED 支路和地线连接，记录电阻阻值。

### 任务 2：命令行控制单路 LED

使用 `gpioinfo` 找到目标 line，用 `gpioset` 点亮和熄灭单路 LED。

### 任务 3：编写流水灯程序

程序支持 `--mode chase|bounce|blink` 和 `--delay-ms` 参数，退出时关闭所有 LED。

## 实验步骤

1. 板端安装工具并查询 GPIO。

```bash
sudo apt update
sudo apt install -y gpiod libgpiod-dev
gpiodetect
gpioinfo
```

2. 用命令验证单路 LED。将 `gpiochipX` 和 `Y` 替换为实际记录值。

```bash
gpioset gpiochipX Y=1
gpioset gpiochipX Y=0
```

3. 主机端编译程序。

```bash
make clean
make TARGET=led_chase
file build/led_chase
```

4. 部署并运行。

```bash
scp build/led_chase <user>@<board-ip>:/tmp/led_chase
ssh <user>@<board-ip> 'chmod +x /tmp/led_chase'
ssh <user>@<board-ip> '/tmp/led_chase --chip gpiochipX --lines 20,21,22,23 --mode chase --delay-ms 200'
ssh <user>@<board-ip> '/tmp/led_chase --chip gpiochipX --lines 20,21,22,23 --mode bounce --delay-ms 120'
```

5. 记录 GPIO 映射表。

```bash
gpioinfo gpiochipX | grep -E 'line +20|line +21|line +22|line +23'
```

## 运行验证

| 验证项 | 预期现象 | 是否通过 |
| --- | --- | --- |
| 接线检查 | 每路 LED 支路通断正确，无短路 |  |
| 单路控制 | `gpioset` 能点亮和熄灭对应 LED |  |
| chase 模式 | 4 路 LED 从左到右循环点亮 |  |
| bounce 模式 | 4 路 LED 来回移动 |  |
| 安全退出 | 程序退出后 LED 全部熄灭 |  |

## 验收记录

| 记录项 | 内容 |
| --- | --- |
| 运行命令 | `gpioinfo`、`gpioset`、`/tmp/led_chase ...` |
| 关键输出 | GPIO chip/line 映射、程序启动参数 |
| 截图或照片 | 接线照片、LED 运行照片或短视频 |
| 异常处理 | 记录不亮、反相、line busy 等问题 |

## 常见问题

| 现象 | 可能原因 | 处理方式 |
| --- | --- | --- |
| LED 不亮 | 极性接反、GPIO line 错误、未共地 | 断电检查接线，重新确认 line |
| LED 常亮不受控 | 接到了固定电源轨而非 GPIO | 重新按 GPIO 输出支路接线 |
| 程序提示权限不足 | 当前用户无 GPIO 访问权限 | 临时使用 `sudo`，或配置 gpio 用户组 |
| Ctrl+C 后 LED 仍亮 | 程序未做退出清理 | 增加信号处理，退出前写 0 |

## 提交要求

- 实验记录：GPIO 映射表、限流电阻阻值、运行命令。
- 运行截图：`gpioinfo` 输出、LED 模式照片或短视频。
- 源码或配置文件：流水灯 C 源码、Makefile。
- 简短说明：说明三种模式的状态切换逻辑。
