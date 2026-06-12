# 实验 1.1：RevyOS 镜像烧录、网络配置与 SSH 登录

## 对应讲次

| 项目 | 内容 |
| --- | --- |
| 章节 | 第 1 章 RISC-V C语言开发与调试 |
| 讲次 | 第 1 讲 |
| 课程主题 | RISC-V 生态与开发环境搭建 |
| 实验类型 | 必做实验 |

大纲讲次原文：第1讲 RISC-V 生态与开发环境搭建。
大纲实验原文：LicheePi 4A 镜像烧录、网络配置与 SSH 登录
大纲知识点原文：RISC-V 指令集架构概述；RuyiSDK 与 开发板矩阵；LicheePi 4A 硬件认知；RevyOS 镜像烧录；首次启动与系统配置；SSH/SCP 远程访问。

## 实验目标

- 能使用课程指定方式将 RevyOS 写入 LicheePi 4A 启动介质。
- 能通过串口或 HDMI 登录 RevyOS，记录系统版本、内核版本和 CPU 架构。
- 能让开发板接入局域网，并从主机端通过 SSH 登录。
- 能使用 SCP 在主机和开发板之间传输文件。

## 对应知识点

| # | 知识点 | 本实验中的验证方式 |
| --- | --- | --- |
| 1 | RISC-V 指令集架构概述 | 板端执行 `uname -m`、`lscpu` 确认 `riscv64` |
| 2 | RuyiSDK 与开发板矩阵 | 主机端记录 `ruyi --version`，说明其在镜像/工具链中的作用 |
| 3 | LicheePi 4A 硬件认知 | 拍照或记录电源、启动介质、串口/HDMI、网口连接 |
| 4 | RevyOS 镜像烧录 | 执行 fastboot/ruyi 烧录并记录镜像文件名和校验值 |
| 5 | 首次启动与系统配置 | 设置时区、网络、主机名，记录 `/etc/os-release` |
| 6 | SSH/SCP 远程访问 | 主机端 SSH 登录并 SCP 上传测试文件 |

## 实验环境

| 项目 | 要求 |
| --- | --- |
| 主机环境 | Linux x86_64，具备 `fastboot`、`ssh`、`scp`、`sha256sum`、可选 `ruyi` |
| 目标板 | LicheePi 4A |
| 目标系统 | RevyOS |
| 硬件连接 | 稳定电源，USB 烧录线，串口线或 HDMI，网线或 Wi-Fi |
| 软件依赖 | 课程指定 RevyOS 镜像、烧录工具、SSH 客户端 |

## 硬件连接或部署关系

| 模块 | 引脚/接口 | 连接说明 | 注意事项 |
| --- | --- | --- | --- |
| 电源 | USB-C 或板卡指定电源口 | 使用满足电流要求的电源适配器 | 供电不稳会导致烧录或启动异常 |
| 烧录连接 | USB 数据口 | 主机连接开发板，进入烧录模式 | 确认 USB 线支持数据传输 |
| 串口终端 | UART 调试口 | GND-GND、TX-RX、RX-TX | 电平使用 3.3V TTL，不接 5V |
| 网络 | Ethernet 或 Wi-Fi | 接入与主机同一局域网 | 记录 IP、网关、DNS |

## 实验任务

### 任务 1：准备镜像和烧录工具

下载课程指定 RevyOS 镜像，记录文件名、版本和校验值，确认主机能识别烧录工具。

### 任务 2：烧录并首次启动

让 LicheePi 4A 进入烧录模式，执行烧录，重启后通过串口或 HDMI 登录 RevyOS。

### 任务 3：配置网络和远程访问

配置开发板网络、启用 SSH，主机端通过 SSH 登录并上传测试文件。

## 实验步骤

1. 主机端检查工具和镜像。

```bash
ruyi --version
fastboot --version
sha256sum revyos-licheepi4a-*.img*
```

2. 进入烧录模式后确认设备。

```bash
fastboot devices
```

3. 按课程镜像说明烧录。不同镜像的分区名称可能不同，以下命令用于记录流程，正式执行以课程镜像发布说明为准。

```bash
fastboot flash ram u-boot-with-spl.bin
fastboot reboot
```

4. 板端首次登录后记录系统信息。

```bash
cat /etc/os-release
uname -a
uname -m
lscpu
hostnamectl
```

5. 配置时区、SSH 和网络检查。

```bash
sudo timedatectl set-timezone Asia/Shanghai
sudo systemctl enable --now ssh
ip -4 addr
ip route
ping -c 3 8.8.8.8
```

6. 主机端登录并传输文件。

```bash
ssh <user>@<board-ip> 'hostname; uname -m'
printf 'rv course ssh test\n' > ssh-test.txt
scp ssh-test.txt <user>@<board-ip>:/tmp/ssh-test.txt
ssh <user>@<board-ip> 'cat /tmp/ssh-test.txt'
```

## 运行验证

能进入 RevyOS shell，输出系统信息、IP 地址，并从 host 通过 SSH 登录。

| 验证项 | 预期现象 | 是否通过 |
| --- | --- | --- |
| 烧录工具识别 | `fastboot devices` 能列出设备 |  |
| RevyOS 启动 | 串口或 HDMI 出现登录界面 |  |
| 系统信息 | `uname -m` 输出 `riscv64` |  |
| 网络连通 | 主机能 ping 通板端 IP |  |
| SSH 登录 | 主机能执行远程命令 |  |
| SCP 传输 | 板端能读取 `/tmp/ssh-test.txt` |  |

## 验收记录

| 记录项 | 内容 |
| --- | --- |
| 运行命令 | `cat /etc/os-release`、`uname -a`、`ip addr`、`ssh <user>@<board-ip>` |
| 关键输出 | RevyOS 版本、内核版本、`riscv64`、板端 IP |
| 截图或照片 | 烧录日志、首次启动界面、SSH 登录界面 |
| 异常处理 | 记录 fastboot、网络或 SSH 失败现象和处理步骤 |

## 常见问题

| 现象 | 可能原因 | 处理方式 |
| --- | --- | --- |
| `fastboot devices` 为空 | 未进入烧录模式、线缆问题、权限问题 | 重新进入烧录模式，换 USB 数据线，检查主机权限 |
| 登录后没有 IP | 网线未连接、DHCP 不可用、Wi-Fi 未配置 | 检查链路灯，使用静态 IP 或配置 Wi-Fi |
| SSH 提示连接拒绝 | ssh 服务未启动 | 板端执行 `sudo systemctl enable --now ssh` |
| SCP 失败 | 用户名、IP、目标路径错误 | 先用 SSH 登录，再确认目标路径权限 |

## 提交要求

- 实验记录：镜像文件名、校验值、系统版本、内核版本、板端 IP。
- 运行截图：烧录工具输出、RevyOS 登录、SSH 登录。
- 源码或配置文件：如修改网络配置，提交对应配置片段。
- 简短说明：描述遇到的问题和解决方式。

