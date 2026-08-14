# Lichee Pi 4A · 课程相关摘录（Markdown）

整理自 Sipeed Wiki，只保留第三章温控风扇实验会用到的信息。  
完整原文见同目录：`LicheePi4A_板卡介绍_wiki.md`、`LicheePi4A_外设使用_wiki.md`、`LicheePi4A_基础上手_wiki.md`，以及 [Wiki 入口](https://wiki.sipeed.com/hardware/zh/lichee/th1520/lp4a.html)。

官方硬件包下载入口（规格书 / 原理图 / 点位图等）在 Wiki「相关链接」；本仓库已缓存规格书 PDF 与插针图。原理图直链有时需登录下载站，失败时请打开：[Sipeed 下载站 LicheePi4A](https://dl.sipeed.com/shareURL/LICHEE/LicheePi4A)。

---

## 1. 板子是什么

- 名称：**Lichee Pi 4A**（底板）+ **Lichee Module 4A**（核心板）
- 主芯片：**TH1520**（RISC-V）
- 本课把它当成一台「能跑 Linux 的小电脑」：插网线 / Wi‑Fi、SSH 登录，再用排针接传感器和继电器

资料来源：[板卡介绍](https://wiki.sipeed.com/hardware/zh/lichee/th1520/lpi4a/1_intro.html)、[lp4a 简介页](https://wiki.sipeed.com/hardware/zh/lichee/th1520/lp4a.html)

---

## 2. 电源与「风扇」别搞混

| 东西 | 是什么 | 本课怎么用 |
|------|--------|------------|
| **12V 电源口** | 整板供电 | 开发板正常上电用 |
| **Fan Power 插座** | 板载散热风扇电源（PWM 可控） | **与实验风扇无关**，不要拿来接 DHT/继电器那套 |
| **20Pin 排针上的 5V / 3.3V / GND** | 给外设供电 | 实验风扇、继电器线圈、传感器、电平转换板从这里取电 |

开箱文档也提到：可用排针 **+5V 与 GND** 测试风扇是否能转——那是供电测试，不是程序控实验风扇的路径。见 [开箱 / 基础上手](https://wiki.sipeed.com/hardware/zh/lichee/th1520/lpi4a/2_unbox.html) 与 `usage.md`。

---

## 3. 20 针排针（本课核心）

官方说明（[外设使用 · GPIO](https://wiki.sipeed.com/hardware/zh/lichee/th1520/lpi4a/6_peripheral.html)）：

- 板载 **2×10** 插针，约 **16** 个原生 IO（含普通 GPIO、几路串口、SPI 等）
- **所有原生 IO 电平是 1.8V**，接 3.3V 模块必须做电平转换
- 串口输入侧有分压，可接常见 3.3V 串口模块（调试用）；GPIO 仍按 1.8V 处理

### 电源脚（有）

- **3.3V**、**5V**、**GND**（两路地）

### 电源脚（没有）

- 排针上 **没有标出的 1.8V 电源脚**  
  → 电平转换板低压侧供电（VCCA）需要另想办法（推荐外接 3.3V→1.8V 小稳压模块）

### 推荐当「普通开关脚」用的四根

| 丝印名 | libgpiod 示例 | 本课建议用途 |
|--------|---------------|--------------|
| **IO1_3** | `/dev/gpiochip1` **line 3** | 继电器信号（风扇） |
| **IO1_4** | line **4** | DHT22 数据线 |
| IO1_5 | line **5** | 备用 |
| IO1_6 | line **6** | 备用 |

官方点灯示例就是打开 `gpiochip1`、取 line 3，对应 **IO1_3**。插针全表见 `lpi4a-wiki-assets/io_map.png` 与 `LicheePi4A_GPIO摘录.md`。

---

## 4. 登录与网络（实验前置）

来自 [基础上手 / usage](https://raw.githubusercontent.com/sipeed/sipeed_wiki/main/docs/hardware/zh/lichee/th1520/lpi4a/usage.md)：

- 串口：底板 **U0-TX / U0-RX**，波特率 **115200**；交叉接、共地
- 早期镜像账号示例：`root` / `sipeed`；部分镜像密码为 `licheepi`（以你盘里镜像说明为准）
- 图形界面快捷开终端：`Ctrl + Alt + T`
- 有线网：插上网线即可；无线：桌面网络图标选 Wi‑Fi
- 查 IP：可装 `net-tools` 后用 `ifconfig`，或用系统网络设置
- SSH：板上有 IP 后，主机 `ssh user@板子IP`（用户名以镜像为准）

本课实验默认：**SSH 已通**，再交叉编译、`scp`、上板运行。

---

## 5. 和本课配件怎么接（逻辑，不是逐步教程）

```
荔枝派 GPIO (1.8V) ──► TXS0108E 低压侧 A
                              │
TXS0108E 高压侧 B (3.3V) ─────┼──► DHT22 DAT
                              └──► 继电器 IN（跳帽拨 H）

排针 3.3V ──► DHT22 VCC、TXS0108E VCCB
排针 5V   ──► 继电器线圈 V+、风扇电源（经继电器 NO–COM）
排针 GND  ──► 所有模块地（必须共地）
TXS0108E VCCA ──► 需要 1.8V（排针没有 → 待补 LDO）
```

- **实验风扇**：走继电器，不走 Fan Power  
- **继电器跳帽**：拨 **H**（高电平吸合），匹配程序 `fan_set(1)`  
- **OE**（转换板使能）：高电平有效（见 `HW-221 产品说明书.pdf`）

---

## 6. 本目录已缓存文件

| 文件 | 内容 |
|------|------|
| `SIPEED_LPi4A_DataSheet_EN_V1.0.pdf` | 官方规格书 |
| `LicheePi4A_*_wiki.md` | Wiki 原文备份 |
| `lpi4a-wiki-assets/` | 插针图、GPIO 编号图等 |
| `LicheePi4A_GPIO摘录.md` | 插针与 gpiod 对照短表 |
| DHT22 / 继电器 / 风扇 / TXS0108E 说明书 | 淘宝页与厂商 PDF |

---

## 7. 仍待拍板（写逐步拼装教程前）

1. **TXS0108E 的 VCCA 1.8V**：是否购买 `3.3V转1.8V LDO`（推荐）  
2. 电平转换板到货后的丝印照片（核对 VCCA/VCCB/OE）  
3. 板上实测确认：`gpiochip` 编号与 IO1_3/IO1_4 方向是否与 Wiki 图一致
