# Ruyi RISC-V Linux Book

Ruyi RISC-V Linux 嵌入式实践课公开材料（**C + RuyiSDK + 真开发板**）。

- **大纲（规划源）：** [`docs/CourseOutline.html`](docs/CourseOutline.html)
- **综合项目：** [`docs/project.html`](docs/project.html)
- **课程说明（定位 / 调研 / 取舍）：** [`docs/intro.md`](docs/intro.md)
- **课程评价标准：** [`docs/course-evaluation-standard.md`](docs/course-evaluation-standard.md)
- **在线预览：** https://duoqilai.github.io/ruyi-riscv-linux-book/

## 课程一句话

六章讲义各三节 + 独立「深入理解」+ 每章一个完整实验（C 脚手架、板上可见验收），最后综合项目：量化出本地模型，在终端里用 DeepSeek Harness 打字，经 MQTT 调用前六章已经写好的风扇 / 灯 / 状态接口。

| 章 | 名称 | 本章实验 |
|----|------|----------|
| ch01 | 环境与工具链 | 上板通道验收（CoreMark 跑分） |
| ch02 | 够用的 C 语言基础 | 模拟温度：写滞回并改读数 |
| ch03 | GPIO 与执行器 | 温控风扇 |
| ch04 | 串口对话与温控 | 串口命令温控 |
| ch05 | 网络与 MQTT | MQTT 远程控灯 |
| ch06 | 线程与协同 | 三线程协同 |
| 综合 | 端侧 Agent 环境终端 | 量化 + 终端 Harness + 荔枝派三线程 |

调研结论摘要见 **`docs/intro.md`**。评价口径见 **`docs/course-evaluation-standard.md`**（CIPP 四层 + OBE 成效写法，用第 1–4 章的板上证据说明）。综合项目正文见 **`docs/project.html`**。

## 研发工作流

```
docs/CourseOutline.html              ← 唯一规划源（先改这里）
docs/project.html                    ← 综合项目正文
docs/intro.md                        ← 课程说明与调研取舍
docs/course-evaluation-standard.md   ← 课程评价标准（CIPP 四层）
         │
         ├── chapters/chXX/lecture.html   ← 讲义：1.1 / 1.2 / 1.3 + 深入理解
         └── chapters/chXX/lab.html       ← 实验：一整块；C 脚手架 + 硬件验收
```

| 文档 | 角色 |
|------|------|
| **CourseOutline** | 改结构、改实验、改术语 → 先改这里 |
| **project.html** | 综合项目：双机、量化、终端 Harness、验收 |
| **intro.md** | 为什么这样设计、调研了什么 |
| **course-evaluation-standard.md** | CIPP 四层 × 12 条，对应第 1–4 章落点 |
| **lecture.html** | 原理 + 单项跟做 + 深入理解 |
| **lab.html** | 半成品往后补；板上现象验收 |

**原则：** 一门课一条线；实验必须结合板上硬件现象。

## 构建与发布

课程页面为静态 HTML。主仓库 `main` 分支已配置自动发布 GitHub Pages。制作时在开发分支改稿，合入 `main` 即发布。

线上入口：https://duoqilai.github.io/ruyi-riscv-linux-book/  
站内链接请用仓库内相对路径（例如 [`docs/CourseOutline.html`](docs/CourseOutline.html)、[`docs/project.html`](docs/project.html)、[`docs/course-evaluation-standard.md`](docs/course-evaluation-standard.md)、[`chapters/ch03/lecture.html`](chapters/ch03/lecture.html)）。GitHub Pages 不会改写写死的绝对 URL。

## 仓库结构

```text
ruyi-riscv-linux-book/
├── README.md
├── docs/
│   ├── CourseOutline.html
│   ├── project.html
│   ├── course-evaluation-standard.md
│   └── intro.md
├── chapters/
│   └── ch01/ … ch06/
├── deck/                     ← 课程介绍幻灯片（含综合项目页）
├── reference/
│   ├── archive/              ← 旧稿（不作为当前讲义）
│   └── boards/               ← 板卡备忘
└── misc/
    └── scripts/
```

## 当前进度

- **CourseOutline / ch01–ch06 讲义与实验：** 已按定稿上线 Pages  
- **综合项目：** `docs/project.html` 已定稿（量化 + 终端 Harness；不再做 tiny 前向）  
- **课程评价标准：** `docs/course-evaluation-standard.md` 已就绪  

发布分支：`main`（GitHub Pages 自动发布）。
