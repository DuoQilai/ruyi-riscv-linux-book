# LicheePi 4A 参考资料

本目录用于收集 LicheePi 4A 相关参考资料，方便编写课程文档、实验指导书和 PPT 时查找板卡信息。这里不维护课程计划，也不替代正式讲义或实验步骤。

## 官方资料

- Sipeed 开箱上手：`https://wiki.sipeed.com/hardware/zh/lichee/th1520/lpi4a/2_unbox.html`
- Sipeed 镜像烧录：`https://wiki.sipeed.com/hardware/zh/lichee/th1520/lpi4a/4_burn_image.html`
- Sipeed 外设使用：`https://wiki.sipeed.com/hardware/zh/lichee/th1520/lpi4a/6_peripheral.html`
- RuyiSDK Support Matrix：`https://github.com/ruyisdk/support-matrix/tree/main/LicheePi4A`

## 可参考信息

- 默认镜像账号可能包含 `debian` / `debian` 和 `sipeed` / `licheepi`，具体以所用镜像说明为准。
- 系统串口通常为 `UART0`，引出为 `U0-RX`、`U0-TX`，需要交叉连接并接 GND。
- 常见串口波特率为 `115200`。
- 进入烧录模式通常需要按住板上 `BOOT` 键，再插入 USB-C 线缆上电。
- Linux 主机可通过 `lsusb` 检查下载模式设备，例如 `ID 2345:7654 T-HEAD USB download gadget`。

## 使用说明

- 课程正文引用本目录内容时，应回到官方资料或实际验证记录确认。
- 截图、命令输出和板卡照片应放入对应章节的 `assets/` 目录。
- 如记录实际验证过程，可单独新增 `notes-YYYY-MM-DD.md`，但不要把本目录写成课程大纲。
