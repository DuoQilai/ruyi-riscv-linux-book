# Ruyi RISC-V Linux Book

Ruyi RISC-V Linux 嵌入式实践课公开材料（C + RuyiSDK + 开发板）。

## 课程研发工作流

```
CourseOutline.html          ← 顶层规划（章节表、小节表、术语、物料、三项目递进）
       │
       ├── ch01/lecture.html + lab.html   ← 已实现
       ├── ch02/lecture.html + lab.html   ← 已实现
       └── ch03–ch06 + 实验 + 综合项目     ← 规划中
```

| 角色 | 看什么 | 何时 |
|------|--------|------|
| 课程研发团队汇报 | `docs/CourseOutline.html` | 讨论章结构、小节拆分、实验大纲 |
| 深入具体章节 | `chapters/chXX/lecture.html` + `lab.html` | 审核讲义内容、实验步骤 |
| 学生 | 同上 HTML + PDF | 自学与实验 |

**约定：** CourseOutline.html 是规划源。每章 lecture.html / lab.html 基于规划实现。规划变更先改 CourseOutline，再同步 lecture/lab。

## 部署到 GitHub Pages

```bash
# 一键部署：把 course content 同步到个人 Pages 仓库
bash scripts/deploy-pages.sh
```

部署目标：`EnzoDing-rgb.github.io/ruyi-riscv-book/`

## 仓库结构

```text
ruyi-riscv-linux-book/
├── README.md
├── scripts/
│   └── deploy-pages.sh        # 部署到 GitHub Pages
├── docs/
│   └── CourseOutline.html     # 课程总大纲（规划源）
├── chapters/
│   ├── ch01/                  # 第一章 开发环境篇
│   │   ├── lecture.html       # 讲义
│   │   ├── lecture.pdf
│   │   ├── lab.html           # 实验
│   │   └── lab.pdf
│   └── ch02/                  # 第二章 工具链与工程
│       ├── lecture.html       # 讲义（含 2.3 选读）
│       ├── lecture.pdf
│       ├── lab.html           # 实验
│       ├── lab.pdf
│       └── code/              # hello、project-template
├── misc/
│   ├── boards/                # 板卡参考资料
│   └── archive/               # 旧 .md 文件、历史规范、模板
└── assets/
    └── styles.css             # 全局样式（规划中）
```

## 怎么读

| 内容 | 路径 |
| --- | --- |
| 课程大纲 | `docs/CourseOutline.html` |
| 第一章讲义 | `chapters/ch01/lecture.html` |
| 第一章实验 | `chapters/ch01/lab.html` |
| 第二章讲义 | `chapters/ch02/lecture.html` |
| 第二章实验 | `chapters/ch02/lab.html` |
| Hello 示例 | `chapters/ch02/code/hello/` |
| LicheePi 4A | `misc/boards/licheepi4a/reference.md` |

## 当前进度

- **ch01**：lecture.html + lab.html + PDF ✅
- **ch02**：lecture.html + lab.html + PDF ✅
- **ch03–ch06 + 实验 1/2 + 综合项目**：规划中（见 CourseOutline.html）

制作分支：`enzo`。
