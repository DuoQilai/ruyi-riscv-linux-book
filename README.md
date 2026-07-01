# Ruyi RISC-V Linux Book

Ruyi RISC-V Linux 嵌入式实践课公开材料（C + RuyiSDK + 开发板）。

## 课程大纲

[`docs/CourseOutline.html`](docs/CourseOutline.html) — 术语表、章节表、BOM、三项目递进。

## 仓库结构

```text
ruyi-riscv-linux-book/
├── README.md                 # 本文件（全仓唯一 README）
├── docs/
│   ├── CourseOutline.html    # 课程总大纲
│   └── archive/              # 历史规范、模板、进度表等
├── chapters/
│   ├── ch01/                 # 第一章 开发环境篇
│   │   ├── slides.html       # 整章幻灯片（1.1–1.3）
│   │   ├── assets/
│   │   └── 1.1/ … 1.3/       # 每节：lecture.md + lab.md
│   └── ch02/                 # 第二章 工具链与工程
│       ├── slides.html
│       ├── code/             # hello、project-template
│       ├── assets/
│       └── 2.1/ … 2.6/
└── boards/                   # 板卡参考资料
    ├── licheepi4a/
    └── k1/
```

## 怎么读

| 内容 | 路径 |
| --- | --- |
| 1.3 讲义 | `chapters/ch01/1.3/lecture.md` |
| 1.3 实验 | `chapters/ch01/1.3/lab.md` |
| 第一章幻灯片 | `chapters/ch01/slides.html` |
| Hello 示例 | `chapters/ch02/code/hello/` |
| LicheePi 4A | `boards/licheepi4a/reference.md` |

## 当前进度

- **ch01**：讲义与实验草稿（`chapters/ch01/`）
- **ch02**：讲义与实验草稿（`chapters/ch02/`，2.6 选读无实验）

制作分支：`enzo`。
