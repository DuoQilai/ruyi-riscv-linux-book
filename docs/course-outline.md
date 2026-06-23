# 课程章节大纲

大纲：https://enzoding-rgb.github.io/riscv-embedded-course/CourseOutline.html

当前课程制作范围为第一章，共 5 个 Class，每个 Class 配套 1 个 Lab。

## 第一章 RISC-V 开发生态与环境准备

| 序号 | Class | 核心知识 | 对应 Lab |
| --- | --- | --- | --- |
| 1.1 | RISC-V 生态与课程目标平台 | ISA、SoC、开发板、操作系统与工具链的层次；LicheePi 4A 硬件接口；课程软硬件边界 | 识别课程平台并建立环境清单 |
| 1.2 | 安装 `ruyi` 并检查工具链 | `ruyi` 的定位、安装与版本检查；软件源更新；工具链发现、安装与基本验证 | 安装 `ruyi` 并完成工具链检查 |
| 1.3 | 镜像烧录与首次启动 | 镜像与启动介质；烧录前确认；首次启动；用户、网络、时间与 apt 软件源检查 | 完成 RevyOS 烧录和首次启动配置 |
| 1.4 | SSH/SCP/串口登录与文件传输 | 串口与网络登录的用途；SSH 主机指纹与密钥；SCP 文件传输；连接故障定位 | 建立串口备用通道和 SSH/SCP 工作通道 |
| 1.5 | `ruyi device provision` 适用边界 | 设备支持查询；自动化 provision 的前提、输出和风险；支持与不支持设备的处理路径 | 查询设备支持并形成 provision 决策记录 |
