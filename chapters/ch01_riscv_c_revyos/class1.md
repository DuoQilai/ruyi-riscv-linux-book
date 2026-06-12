# 1.1 RISC-V 生态与开发环境搭建

## 对应大纲

大纲讲次原文：第1讲 RISC-V 生态与开发环境搭建。
大纲知识点原文：RISC-V 指令集架构概述；RuyiSDK 与 开发板矩阵；LicheePi 4A 硬件认知；RevyOS 镜像烧录；首次启动与系统配置；SSH/SCP 远程访问。

RISC-V 指令集架构、RuyiSDK 与开发板矩阵、LicheePi 4A 硬件认知、RevyOS 镜像烧录、首次启动与系统配置、SSH/SCP 远程访问。

## 目标

学生能进入 LicheePi 4A 的 RevyOS shell，完成基础系统配置，确认网络连通，并记录板卡、系统和远程访问关键信息。

## 知识点

| # | 知识点 | 本讲说明 |
| --- | --- | --- |
| 1 | RISC-V 指令集架构概述 | 认识 RV64GC、模块化扩展、特权级和 RISC-V 与 x86/ARM 的差异 |
| 2 | RuyiSDK 与开发板矩阵 | 了解 RuyiSDK 在工具链、镜像和软件包管理中的作用，对比常见 RISC-V 板卡定位 |
| 3 | LicheePi 4A 硬件认知 | 识别 TH1520、内存/存储、USB、HDMI、Ethernet、40pin 扩展接口 |
| 4 | RevyOS 镜像烧录 | 理解 fastboot/ruyi 烧录流程、启动介质和基础分区结构 |
| 5 | 首次启动与系统配置 | 完成登录、网络、apt 源、locale、时区和主机名设置 |
| 6 | SSH/SCP 远程访问 | 配置 sshd、密钥登录、SCP 文件传输和串口终端备用通道 |

## 讲授要点

- 先区分“架构、SoC、开发板、操作系统、工具链”五个层次，避免把 RISC-V 等同于某一块板。
- LicheePi 4A 使用 TH1520，课程统一以 RevyOS 为目标系统，后续所有命令和实验都以板端 Linux 用户态为主。
- 镜像烧录前必须确认设备节点和目标介质，烧录后优先通过串口或 HDMI 验证启动，再配置网络和 SSH。
- SSH 免密登录不是为了省略密码，而是为了让后续脚本化部署、GDB 远程调试和 systemd 验证可重复。
- 系统信息记录是实验资产：镜像版本、内核版本、IP 地址、用户名、工具版本后续都会用于排错。

## 操作或演示

1. 在主机端准备镜像与工具，确认下载文件校验值和 fastboot/ruyi 可用。

```bash
ruyi --version
fastboot --version
sha256sum revyos-licheepi4a-*.img*
```

2. 将开发板进入烧录模式，按实际镜像说明执行烧录；演示时强调确认设备而不是机械复制命令。

```bash
fastboot devices
fastboot flash ram u-boot-with-spl.bin
fastboot reboot
```

3. 首次进入 RevyOS 后记录系统信息。

```bash
cat /etc/os-release
uname -a
lscpu
ip addr
```

4. 配置网络、时区和 SSH 访问。

```bash
sudo timedatectl set-timezone Asia/Shanghai
sudo systemctl enable --now ssh
ssh-keygen -t ed25519 -C "rv-course"
ssh-copy-id <user>@<board-ip>
ssh <user>@<board-ip> 'hostname; uname -m; ip -4 addr'
```

## 运行验证

| 验证项 | 命令 | 预期现象 |
| --- | --- | --- |
| 系统版本 | `cat /etc/os-release` | 显示 RevyOS 发行版信息 |
| 架构确认 | `uname -m` | 输出 `riscv64` |
| 网络连通 | `ping <board-ip>` | 主机能收到 ICMP 响应 |
| SSH 登录 | `ssh <user>@<board-ip> 'hostname'` | 无需手输密码或能稳定登录 |
| 文件传输 | `scp hello.txt <user>@<board-ip>:/tmp/` | 板端 `/tmp/hello.txt` 存在 |

## 常见问题

| 现象 | 可能原因 | 处理方式 |
| --- | --- | --- |
| fastboot 看不到设备 | 未进入烧录模式、USB 线仅供电、权限不足 | 换数据线，重新进入烧录模式，Linux 主机检查 udev 权限 |
| HDMI 无画面 | 镜像未启动、显示器兼容问题、供电不足 | 优先接串口查看启动日志，确认电源规格 |
| SSH 连接拒绝 | sshd 未启动、网络不通、防火墙限制 | `sudo systemctl status ssh`，检查 `ip addr` 和同网段配置 |
| apt 更新失败 | 源不可达、DNS 未配置、时间错误 | 检查 `/etc/resolv.conf`、`timedatectl`，必要时更换 RevyOS 可用源 |

## 本讲成果

- 完成一台 LicheePi 4A 的 RevyOS 启动和远程访问。
- 形成板卡信息记录表：镜像版本、内核版本、IP、登录用户、连接方式。
- 为后续交叉编译、远程调试和自动化部署准备稳定目标环境。

