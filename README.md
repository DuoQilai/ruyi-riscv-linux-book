# Ruyi RISC-V Linux Book

Ruyi RISC-V Linux 嵌入式实践课公开材料（**C + RuyiSDK + 真开发板**）。

- **大纲（规划源）：** [`docs/CourseOutline.html`](docs/CourseOutline.html)
- **课程说明（定位 / 调研 / 取舍）：** [`docs/intro.md`](docs/intro.md)
- **在线预览：** https://enzoding-rgb.github.io/ruyi-riscv-book/

## 课程一句话

六章讲义各三节 + 每章一个完整实验（C 脚手架、板上可见验收），最后综合项目：老师提供 `weights.h`，学生手写 tiny 前向并接到外设 / MQTT。

| 章 | 名称 | 本章实验 |
|----|------|----------|
| ch01 | 环境与工具链 | 上板通道验收（CoreMark 硬性跑分） |
| ch02 | 够用的 C | 假温控改代码闯关 |
| ch03 | GPIO / UART / 外设 | 温控风扇 |
| ch04 | 文件 / 配置 / 日志 | 配置化温控（配置 + 历史 + 告警） |
| ch05 | 网络与 MQTT | MQTT 远程控灯 |
| ch06 | 线程与协同 | 采集∥控制 + Ctrl+C 干净停 |
| 综合 | 智能环境终端 | 总装 + 边缘推理部署 |

调研结论摘要（`reference/` 采纳/舍弃、NJU PA 借鉴边界）见 **`docs/intro.md`**。

## 研发工作流

```
docs/CourseOutline.html     ← 唯一规划源（先改这里）
docs/intro.md               ← 课程说明与调研取舍
         │
         ├── chapters/chXX/lecture.html   ← 讲义：固定 1.1 / 1.2 / 1.3
         └── chapters/chXX/lab.html       ← 实验：一整块；C 脚手架 + 硬件验收
```

| 文档 | 角色 |
|------|------|
| **CourseOutline** | 改结构、改实验、改术语 → 先改这里 |
| **intro.md** | 为什么这样设计、调研了什么 |
| **lecture.html** | 原理 + 单项跟做 |
| **lab.html** | 半成品往后补；不重贴讲义命令块 |

**原则：** 一门课一条线；无选修双轨；实验必须结合板上硬件现象（有别于纯 CSAPP Shell 作业）。

## 构建与发布

```bash
# 完整流程（PDF + 推送课程仓 + 部署 Pages）在 misc/scripts/build.sh
# 仅同步大纲到 Pages 时可手动拷贝 docs/CourseOutline.html
bash misc/scripts/build.sh "update message"
```

线上入口：`https://enzoding-rgb.github.io/ruyi-riscv-book/`  
大纲直链：`https://enzoding-rgb.github.io/ruyi-riscv-book/CourseOutline.html`

## 仓库结构

```text
ruyi-riscv-linux-book/
├── README.md
├── docs/
│   ├── CourseOutline.html
│   ├── intro.md
│   └── index.html
├── chapters/
│   ├── ch01/ … ch03/     # 讲义/实验初稿（将按新大纲对齐）
│   └── …
├── reference/            # 调研资料（笔记等；大书默认不进主线）
└── misc/
    ├── scripts/build.sh
    ├── boards/
    └── archive/
```

## 当前进度

- **CourseOutline：** ch01–ch06 + 综合已按定稿更新 ✅  
- **ch01–ch03：** 有讲义/实验 HTML 初稿；待按新三节结构重写对齐  
- **ch04–ch06 + 综合正文：** 规划已定，待开发  

制作分支：`enzo`。
