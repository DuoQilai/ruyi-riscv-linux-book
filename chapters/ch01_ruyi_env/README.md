# 第一章 RISC-V 开发生态与环境准备

本章建立后续课程所需的统一开发环境。学生将先认识 RISC-V 软件与硬件生态，再完成 `ruyi`、工具链、RevyOS、串口、SSH 和文件传输配置，最后判断目标设备是否适合使用 `ruyi device provision`。

图片、截图和图表素材存放在 `assets/` 目录，并按 `docs/templates/asset-guidelines.md` 的规则命名和引用。

| Class | 主题 | 学习成果 | 对应 Lab |
| --- | --- | --- | --- |
| Class 1 | RISC-V 生态与课程目标平台 | 建立平台分层认知和个人设备清单 | Lab 1 |
| Class 2 | 安装 `ruyi` 并检查工具链 | 获得可验证的工具管理入口 | Lab 2 |
| Class 3 | 镜像烧录与首次启动 | 获得可启动、可联网的 RevyOS 系统 | Lab 3 |
| Class 4 | SSH/SCP/串口登录与文件传输 | 建立稳定的远程开发通道 | Lab 4 |
| Class 5 | `ruyi device provision` 适用边界 | 能为具体设备选择自动或手工准备方式 | Lab 5 |

## 本章成果

- 一份课程目标平台与个人设备清单。
- 一份 `ruyi` 和工具链版本检查记录。
- 一份 RevyOS 镜像、首次启动和网络配置记录。
- 一套串口、SSH 与 SCP 连接验证记录。
- 一份设备 provision 支持判断与风险说明。
