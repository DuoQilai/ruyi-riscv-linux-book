# K1 / Muse Pi Pro 扩展板卡

本课程中的 K1 指 [Muse Pi Pro](https://www.spacemit.com/spacemit-muse-pi-pro/)。

在新版 6 章大纲中，课程主线开发板已经调整为 LicheePi 4A + RevyOS。K1 / Muse Pi Pro 资料继续保留，用于扩展适配、板卡对比和后续可能的增强实验，不作为课程默认目标板或最终验收板。

课程正文应优先使用 LicheePi 4A + RevyOS 的实际流程。涉及 K1 / Muse Pi Pro 的镜像、引脚、设备路径和外设连接差异，统一放在本目录下说明，避免把主线课程写回多板适配手册。

## 关键文件

- `board-selection.md`：K1 / Muse Pi Pro 扩展适配说明

## 已找到的资料来源

- RuyiSDK Support Matrix：`https://github.com/ruyisdk/support-matrix/tree/main/Muse_Pi_Pro`
- Bianbu 测试记录：`https://github.com/ruyisdk/support-matrix/blob/main/Muse_Pi_Pro/Bianbu/README_zh.md`
- openEuler 测试记录：`https://github.com/ruyisdk/support-matrix/blob/main/Muse_Pi_Pro/openEuler/README_zh.md`
- SpacemiT 产品页：`https://www.spacemit.com/spacemit-muse-pi-pro/`

## 当前可确认信息

- RuyiSDK Support Matrix 中已有 Muse Pi Pro 目录，并包含 Bianbu 与 openEuler 测试记录。
- Bianbu 记录对应 Bianbu-Computer UEFI v1.3，基于 Bianbu Star 2.1。
- Bianbu 记录给出默认串口登录账号：`root`，默认密码：`bianbu`。
- openEuler 记录对应 openEuler 24.03-LTS-SP1，给出 SPI NOR 固件刷写、microSD 镜像写入和串口登录记录。
- SpacemiT 产品页给出板卡规格：M1 处理器、8GB/16GB LPDDR4X、64GB/128GB eMMC、千兆以太网、USB3.0、UART TTL 调试接口、40Pin GPIO、MIPI DSI/CSI、M.2、miniPCIe 等。

## 适配边界

- K1 / Muse Pi Pro 可作为 LicheePi 4A 之外的扩展验证板。
- K1 / Muse Pi Pro 适配材料应标注“扩展/参考”，不得覆盖 LicheePi 4A + RevyOS 的默认实验步骤。
- 如某项实验在 K1 / Muse Pi Pro 上更适合演示，可以作为附录或拓展路径补充。
- 第 4 章默认验收仍是 LicheePi 4A + RevyOS 上的手机 MQTT 远程灯光控制。
- 第 6.3 TinyML KWS 可参考 K1 / Muse Pi Pro 的音频或 AI 资源，但默认讲次归属和验收口径不变。

## 仍需实测

- K1 / Muse Pi Pro 可用系统镜像、烧录步骤和登录记录。
- `ruyi device provision` 是否支持 Muse Pi Pro，以及完整命令记录。
- SSH/SCP/串口登录记录。
- `gpioinfo`、`gpiodetect`、`gpioset --version`、`gpioget --version` 输出。
- LED、按键、PWM、ADC、I2C/SPI、OLED、传感器、摄像头和麦克风的实际设备路径。

实测结果应保存为 `boards/k1/validation-YYYY-MM-DD.md`，并保留原始命令输出、截图或照片。
