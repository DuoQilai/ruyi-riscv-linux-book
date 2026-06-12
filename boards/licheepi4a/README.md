# LicheePi 4A 主线开发板

LicheePi 4A 是新版 6 章课程的主线开发板，课程正文、实验指导、slides 和阶段项目默认围绕 LicheePi 4A + RevyOS 展开。

本目录维护 LicheePi 4A 的镜像烧录、首次启动、串口、SSH、GPIO/UART/PWM/ADC/I2C/SPI、OLED、DHT22、USB 摄像头和其他外设验证记录。K1 / Muse Pi Pro 如需保留，只作为扩展板卡或参考适配，不改变本目录的主线地位。

## 覆盖范围

| 新版章节 | LicheePi 4A + RevyOS 验证重点 | 说明 |
| --- | --- | --- |
| ch01 RISC-V C语言开发与调试 | RevyOS 镜像烧录、首次启动、网络配置、SSH/SCP、工具链、GDB、Shell 部署 | 建立开发闭环 |
| ch02 GPIO、UART、常用外设 | 面包板、LED、按键、UART、PWM、ADC、I2C/SPI、OLED、DHT22、温控风扇 | 完成第 2 章温控风扇阶段项目 |
| ch03 程序库、进程与线程 | 库封装、守护进程、IPC、多线程传感器终端 | 与具体板卡关系较弱，但运行记录应以 RevyOS 为准 |
| ch04 文件系统与网络编程 | 文件 I/O、配置、日志、TCP Server、HTTP、MQTT/mosquitto | 完成第 4 章手机 MQTT 远程灯光控制阶段项目 |
| ch05 RISC-V 向量扩展 | C910 RVV 能力、编译选项、intrinsics、性能记录 | 注意 TH1520/C910 的 RVV 版本和工具链兼容性 |
| ch06 边缘智能 | USB 摄像头/OpenCV、目标检测、TinyML KWS、LLM/VLM 轻量演示 | TinyML KWS 对应第 6 章第 3 讲 |

## 需要补齐

- RevyOS 镜像来源、版本和烧录步骤。
- 默认用户名、密码、网络配置和 SSH 登录方式。
- 串口设备名、波特率、接线方式和权限说明。
- GPIO、UART、PWM、ADC、I2C/SPI 设备路径。
- LED、按键、OLED、DHT22、风扇/继电器或 MOSFET、麦克风和摄像头的实际连接方式。
- 第 2 章温控风扇、第 4 章 MQTT 灯光、第 6.3 TinyML KWS 的运行记录。
- 已完成验证的命令记录、截图或日志。

## 已找到的资料来源

- Sipeed 开箱上手：`https://wiki.sipeed.com/hardware/zh/lichee/th1520/lpi4a/2_unbox.html`
- Sipeed 镜像烧录：`https://wiki.sipeed.com/hardware/zh/lichee/th1520/lpi4a/4_burn_image.html`
- Sipeed 外设使用：`https://wiki.sipeed.com/hardware/zh/lichee/th1520/lpi4a/6_peripheral.html`
- RuyiSDK Support Matrix：`https://github.com/ruyisdk/support-matrix/tree/main/LicheePi4A`

## 当前可确认信息

- 默认镜像账号包含 `debian` / `debian` 和 `sipeed` / `licheepi`，`root` 默认没有设置密码。
- 系统串口是 `UART0`，引出为 `U0-RX`、`U0-TX`，需要交叉连接并接 GND。
- 系统串口波特率为 `115200`。
- 烧录模式入口：按住板上 `BOOT` 键，再插入 USB-C 线缆上电。
- Linux 主机可用 `lsusb` 识别 `ID 2345:7654 T-HEAD USB download gadget`。
- 外设页给出 sysfs GPIO、libgpiod、UART、I2C、SPI、USB 摄像头和 CSI 摄像头的使用线索。

## 仍需实测

- 课程实际使用的 RevyOS 镜像名称、版本、下载路径和烧录日志。
- SSH、SCP、串口登录记录。
- `libgpiod` 命令版本和参数口径。
- LED/Button GPIO chip、line 和 active level。
- 板载 LED 是否走 `/sys/class/leds/`，板载按键是否走 `/dev/input/event*`。
- MQTT broker、topic、payload、PWM 调光和状态回传的完整验证。
- TinyML KWS 的音频输入、文件 fallback、推理耗时和灯光动作验证。

实测结果应保存为 `boards/licheepi4a/validation-YYYY-MM-DD.md`，并保留原始命令输出、截图或照片。
