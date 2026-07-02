# Ruyi RISC-V Linux Book

Ruyi RISC-V Linux 嵌入式实践课公开材料（C + RuyiSDK + 开发板）。

## 仓库结构

```text
ruyi-riscv-linux-book/
├── index.html                 # 课程首页（规划中）
├── README.md
├── docs/
│   └── CourseOutline.html     # 课程总大纲
├── chapters/
│   ├── ch01/                  # 第一章 开发环境篇
│   │   ├── lecture.html       # 讲义（阅读材料 + 投屏两用）
│   │   ├── lecture.pdf        # 讲义 PDF
│   │   ├── lab.html           # 合并实验（对应 1.1–1.3）
│   │   ├── lab.pdf            # 实验 PDF
│   │   └── slides/            # HTML slide deck（1920×1080）
│   └── ch02/                  # 第二章 工具链与工程
│       ├── lecture.html       # 讲义
│       ├── lecture.pdf
│       ├── lab.html           # 合并实验（对应 2.1–2.2）
│       ├── lab.pdf
│       ├── slides/            # HTML slide deck
│       └── code/              # hello、project-template
├── misc/
│   ├── boards/                # 板卡参考资料
│   │   ├── licheepi4a/
│   │   ├── k1/
│   │   └── riscv-ai-boards-2025-2026.md
│   └── archive/               # 历史规范、模板、进度表
└── assets/
    └── styles.css             # 全局样式
```

## 怎么读

| 内容 | 路径 |
| --- | --- |
| 第一章讲义 | `chapters/ch01/lecture.html` |
| 第一章实验 | `chapters/ch01/lab.html` |
| 第一章幻灯片 Deck | `chapters/ch01/slides/` |
| 第二章讲义 | `chapters/ch02/lecture.html` |
| 第二章实验 | `chapters/ch02/lab.html` |
| Hello 示例 | `chapters/ch02/code/hello/` |
| LicheePi 4A | `misc/boards/licheepi4a/reference.md` |
| 课程大纲 | `docs/CourseOutline.html` |

## 当前进度

- **ch01**：讲义、实验框架待迁移为 HTML；幻灯片 Deck 已完成（15 页）
- **ch02**：讲义、实验框架待迁移为 HTML；幻灯片待制作

制作分支：`enzo`。
