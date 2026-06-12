# K1 / Muse Pi Pro 扩展适配说明

## 结论

本课程中的 K1 指 [Muse Pi Pro](https://www.spacemit.com/spacemit-muse-pi-pro/)。在新版 6 章大纲中，课程主线开发板是 LicheePi 4A，目标系统是 RevyOS。K1 / Muse Pi Pro 不再作为课程默认目标板或最终验收板，只作为扩展板卡、资料参考和后续适配方向保留。

QEMU 继续作为低硬件依赖内容的辅助环境，可用于 ch01、文件、线程等前置验证；它同样不改变 LicheePi 4A + RevyOS 主线。

## 适配原则

- 课程正文、slides 和实验指导默认使用 LicheePi 4A + RevyOS。
- K1 / Muse Pi Pro 只进入附录、扩展路径或板卡对比说明。
- 不同时维护多块板卡主线。
- 不为了扩展板卡适配改变 6 章 24 讲结构。
- 涉及镜像烧录、登录账号、串口设备、GPIO 编号、PWM/ADC/I2C/SPI 路径等差异，统一放入 `boards/`。
- 样章或实验截图需要标注实际运行环境，例如 `LicheePi 4A + RevyOS`、`K1 / Muse Pi Pro 扩展验证` 或 `QEMU virt`。

## K1 / Muse Pi Pro 的参考价值

| 维度 | 说明 |
| --- | --- |
| 板卡对比 | 可用于解释不同 RISC-V Linux 开发板在镜像、外设和生态上的差异 |
| AI 扩展 | 可作为 TinyML、视觉 AI 或本地模型部署的后续增强参考 |
| 外设适配 | 可补充 GPIO、UART、I2C/SPI、摄像头、音频等扩展验证记录 |
| 课程延展 | 可服务于进阶班、附录或二期材料，不影响当前 6 章主线 |

## 默认实验包口径

| 类别 | 新版默认器件 |
| --- | --- |
| 主板 | LicheePi 4A |
| 系统 | RevyOS |
| 基础连接 | 电源、数据线、串口线、网线 |
| GPIO | LED、按键、面包板、杜邦线 |
| 常用外设 | PWM LED、ADC 输入模块、OLED |
| 传感器 | DHT22 或同类温湿度传感器 |
| 执行器 | 继电器模块、风扇模块或 MOSFET 驱动 |
| 网络项目 | 手机 MQTT 客户端、mosquitto、灯光模块 |
| 边缘智能 | USB 摄像头、麦克风或预录音频文件 |

K1 / Muse Pi Pro 可另列扩展实验包，不进入默认采购和验收口径。

## 当前验证策略

| 环境 | 定位 | 使用边界 |
| --- | --- | --- |
| LicheePi 4A + RevyOS | 课程主线 | 6 章 24 讲、真实外设、阶段项目和默认验收 |
| K1 / Muse Pi Pro | 扩展参考 | 板卡对比、后续适配、AI 或外设增强，不覆盖主线 |
| QEMU `virt` | 辅助环境 | ch01、文件、线程等低硬件依赖内容的前置验证 |

## QEMU、扩展板与主线板分工

| 内容 | 默认环境 |
| --- | --- |
| RuyiSDK 安装 | 主机 |
| RevyOS 镜像烧录和首次启动 | LicheePi 4A |
| Hello World、ELF、Makefile | LicheePi 4A + RevyOS，QEMU 可前置验证 |
| 文件、配置、日志 | LicheePi 4A + RevyOS，QEMU 可前置验证 |
| 线程与任务协同 | LicheePi 4A + RevyOS，QEMU 可前置验证 |
| GPIO、UART、PWM、ADC、OLED、DHT22 | LicheePi 4A + RevyOS |
| 第 2 章温控风扇 | LicheePi 4A + RevyOS |
| 第 4 章手机 MQTT 远程灯光控制 | LicheePi 4A + RevyOS |
| 第 5 章 RVV 加速 | LicheePi 4A + RevyOS |
| 第 6.3 TinyML KWS | LicheePi 4A + RevyOS，K1 / Muse Pi Pro 可参考扩展 |

## 不采用多板主线的原因

暂不把 K1 / Muse Pi Pro、Duo、Duo256、K230、BPI-F3 等开发板同时纳入主线。多板支持可以作为附录或拓展适配，不影响 LicheePi 4A + RevyOS 主线。

原因：

- 会增加课件、代码、实验和答疑成本。
- 会导致学生运行结果不一致。
- 会拖慢 24 讲课程材料交付。
- 当前新版大纲已经明确目标平台和系统口径。

## 待补充信息

- LicheePi 4A + RevyOS 课程镜像、烧录步骤和登录记录。
- LicheePi 4A GPIO/UART/I2C/SPI/PWM/ADC 引脚和设备路径。
- 第 2 章温控风扇、第 4 章 MQTT 灯光、第 6.3 TinyML KWS 的板端验证记录。
- K1 / Muse Pi Pro 如继续适配，应补充为扩展验证记录，而不是主线替代说明。
