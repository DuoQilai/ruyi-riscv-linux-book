# 课程章节大纲

完整课程设计见 [`CourseOutline.html`](CourseOutline.html)（术语表、三项目递进、各章小节、BOM、硬规则）。

当前制作范围：**第一章 + 第二章**。

## 第一章 开发环境篇（5 节）

| 节 | 主题 | 核心知识 | 实验 |
| --- | --- | --- | --- |
| 1.1 | RISC-V 生态与课程目标平台 | ISA、SoC、开发板、OS、工具链层次；LicheePi 4A | `ch01_ruyi_env/lab/1.1.md` |
| 1.2 | 安装 `ruyi` 并检查工具链 | 安装、版本、软件源、工具链验证 | `lab/1.2.md` |
| 1.3 | 镜像烧录与首次启动 | 烧录、首次启动、网络、用户、apt 源 | `lab/1.3.md` |
| 1.4 | SSH/SCP/串口登录与文件传输 | 串口、SSH、SCP、故障定位 | `lab/1.4.md` |
| 1.5 | `ruyi device provision` 适用边界 | 设备支持查询、自动化边界与风险 | `lab/1.5.md` |

**章成果**：RuyiSDK 环境 + 板子/QEMU 能进系统 + SSH/串口/SCP 稳定。

## 第二章 工具链与工程（6 节，2.6 选读）

| 节 | 主题 | 核心知识 | 实验 |
| --- | --- | --- | --- |
| 2.1 | host、target、sysroot | 交叉编译三要素 | `ch02_toolchain/lab/2.1.md` |
| 2.2 | 第一个 C 程序 | 编译 → 传输 → 运行 | `lab/2.2.md` |
| 2.3 | `readelf`、`ldd` | ELF 与动态库观察 | `lab/2.3.md` |
| 2.4 | Makefile | 规则、变量、`clean` | `lab/2.4.md` |
| 2.5 | 统一工程目录 | `src`/`include`/`build` | `lab/2.5.md` |
| 2.6 | CoreMark（选读） | 算力边界对比 | — |

**章成果**：最小可部署 C 工程 + Makefile 模板（`code/ch02/`）。
