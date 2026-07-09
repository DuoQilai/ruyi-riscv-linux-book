# Ruyi RISC-V Linux Book

Ruyi RISC-V Linux 嵌入式实践课公开材料（C + RuyiSDK + 开发板）。

## 课程研发工作流

```
docs/CourseOutline.html     ← 唯一规划源（章/节目标、配套实验、术语、BOM）
         │
         ├── chapters/chXX/lecture.html   ← 讲义：按节 1.1 / 1.2 / … 组织，教 + 可跟做
         └── chapters/chXX/lab.html       ← 实验：一章一篇，递进于讲义，不拆节号
```

| 文档 | 角色 |
|------|------|
| **CourseOutline** | 改结构、改实验归属、改术语 → **先改这里** |
| **lecture.html** | 概念、图示、操作示范、课堂练习；节与 outline 对齐 |
| **lab.html** | 讲义跑通后的整合验收 / 对比 / 决策 / 独立动手（如 ch02 CoreMark） |

**原则：** 讲义能自学跟做；实验不重复讲义命令块，只递进一层。无独立交付价值时不硬拆 lab。

## 构建与发布

```bash
# 生成 PDF → 提交 enzo → 部署 GitHub Pages
bash scripts/build.sh "update message"
```

`scripts/build.sh` 会：

1. 用 Playwright 为章节 HTML（ch01–ch03 的 lecture + lab）生成 PDF  
2. `git commit` 并 `push origin enzo`  
3. 将 `CourseOutline.html` 与章节 HTML/PDF（及 `ch02/code/`）同步到 Pages 仓库  

线上入口（`index.html` → 大纲）：`https://enzoding-rgb.github.io/ruyi-riscv-book/`

## 仓库结构

```text
ruyi-riscv-linux-book/
├── README.md
├── scripts/
│   └── build.sh
├── docs/
│   └── CourseOutline.html
├── chapters/
│   ├── ch01/
│   │   ├── lecture.html / lecture.pdf
│   │   └── lab.html / lab.pdf
│   ├── ch02/
│   │   ├── lecture.html / lecture.pdf
│   │   ├── lab.html / lab.pdf
│   │   └── code/              # hello、project-template
│   └── ch03/
│       ├── lecture.html
│       ├── lab.html
│       └── code/              # blink、uart-echo、temp-fan
└── misc/
    ├── boards/
    └── archive/
```

## 怎么读

| 内容 | 路径 |
| --- | --- |
| 课程大纲 | `docs/CourseOutline.html` |
| 第一章讲义 / 实验 | `chapters/ch01/lecture.html` · `lab.html` |
| 第二章讲义 / 实验 | `chapters/ch02/lecture.html` · `lab.html` |
| 第三章讲义 / 实验 | `chapters/ch03/lecture.html` · `lab.html` |
| Hello 示例 | `chapters/ch02/code/hello/` |
| 温控风扇示例 | `chapters/ch03/code/temp-fan/` |

## 当前进度

- **ch01**：outline + lecture + lab（环境连通与一键准备决策）✅  
- **ch02**：outline + lecture + lab（CoreMark 跑分）✅  
- **ch03**：outline（3 节）+ lecture + lab（实验 1 温控风扇）+ code ✅  
- **ch04–ch06 + 实验 2 + 综合项目**：见 CourseOutline  

制作分支：`enzo`。
