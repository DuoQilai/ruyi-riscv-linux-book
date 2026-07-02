# Slides

幻灯片使用 **HTML**（非 pptx），代码友好，可单页长文或按节分文件。

## 约定

| 项目 | 说明 |
| --- | --- |
| 位置 | `slides/ch01/`、`slides/ch02/` … |
| 命名 | `1.1.html` 与节号对齐，或 `ch01.html` 整章合一 |
| 内容来源 | 必须来自对应节讲义，不引入讲义未解释的概念 |
| 页数 | 无硬上限；以讲清 3–5 个知识点为准 |

## 第一章

| 节 | 主题 | 建议文件 | 状态 |
| --- | --- | --- | --- |
| 1.1 | RISC-V 生态与课程目标平台 | `slides/ch01/1.1.html` | 待制作 |
| 1.2 | 安装 ruyi 并检查工具链 | `slides/ch01/1.2.html` | 待制作 |
| 1.3 | 镜像烧录与首次启动 | `slides/ch01/1.3.html` | 待制作 |
| 1.4 | SSH/SCP/串口登录与文件传输 | `slides/ch01/1.4.html` | 待制作 |
| 1.5 | ruyi device provision 适用边界 | `slides/ch01/1.5.html` | 待制作 |

## 第二章

| 节 | 主题 | 建议文件 | 状态 |
| --- | --- | --- | --- |
| 2.1 | host、target、sysroot | `slides/ch02/2.1.html` | 待制作 |
| 2.2 | 第一个 C 程序 | `slides/ch02/2.2.html` | 待制作 |
| 2.3 | readelf、ldd | `slides/ch02/2.3.html` | 待制作 |
| 2.4 | Makefile | `slides/ch02/2.4.html` | 待制作 |
| 2.5 | 统一工程目录 | `slides/ch02/2.5.html` | 待制作 |
| 2.6 | CoreMark（选读） | `slides/ch02/2.6.html` | 可选 |

## HTML 幻灯片最低要求

- 封面：标题、本节目标、学习成果。
- 正文：知识点 + 图表/截图/代码片段（完整代码放 `code/`）。
- 实验页：链到 `lab/X.Y.md` 的要点与验收。
- 小结：成果、常见错误、与下一节衔接。

本地预览：浏览器直接打开 HTML，或用任意静态服务器。
