# Ruyi RISC-V Linux Book

Ruyi RISC-V Linux 嵌入式实践课公开材料（**C + RuyiSDK + 真开发板**）。

- **大纲（规划源）：** [`docs/CourseOutline.html`](docs/CourseOutline.html)
- **课程说明（定位 / 调研 / 取舍）：** [`docs/intro.md`](docs/intro.md)
- **课程评价标准（课题组自评）：** [`docs/course-evaluation-standard.md`](docs/course-evaluation-standard.md)
- **在线预览：** https://enzoding-rgb.github.io/ruyi-riscv-book/

## 课程一句话

六章讲义各三节 + 独立「深入理解」+ 每章一个完整实验（C 脚手架、板上可见验收），最后综合项目：老师提供 `weights.h`，学生手写 tiny 前向并接到外设 / MQTT。

| 章 | 名称 | 本章实验 |
|----|------|----------|
| ch01 | 环境与工具链 | 上板通道验收（CoreMark 跑分） |
| ch02 | 够用的 C 语言基础 | 假温控：写滞回并改读数 |
| ch03 | GPIO 与执行器 | 温控风扇 |
| ch04 | 串口对话与温控 | 串口命令温控 |
| ch05 | 网络与 MQTT | MQTT 远程控灯 |
| ch06 | 线程与协同 | 三线程协同 |
| 综合 | 智能环境终端 | 异常联动总验收 |

调研结论摘要见 **`docs/intro.md`**。评价口径见 **`docs/course-evaluation-standard.md`**（CIPP 四层结构 + OBE 成效写法，课题组 10 分制自评）。

## 研发工作流

```
docs/CourseOutline.html              ← 唯一规划源（先改这里）
docs/intro.md                        ← 课程说明与调研取舍
docs/course-evaluation-standard.md   ← 课程质量自评标准
         │
         ├── chapters/chXX/lecture.html   ← 讲义：1.1 / 1.2 / 1.3 + 深入理解
         └── chapters/chXX/lab.html       ← 实验：一整块；C 脚手架 + 硬件验收
```

| 文档 | 角色 |
|------|------|
| **CourseOutline** | 改结构、改实验、改术语 → 先改这里 |
| **intro.md** | 为什么这样设计、调研了什么 |
| **course-evaluation-standard.md** | 四部分 × 12 条、10 分制自评 |
| **lecture.html** | 原理 + 单项跟做 + 深入理解 |
| **lab.html** | 半成品往后补；板上现象验收 |

**原则：** 一门课一条线；实验必须结合板上硬件现象。

## 构建与发布

在 `enzo` 分支改完后正常 `git commit` / `git push` 即可。个人预览站需要更新时，把课程页面拷到个人 Pages 仓库对应目录。

线上入口：https://enzoding-rgb.github.io/ruyi-riscv-book/  
大纲：https://enzoding-rgb.github.io/ruyi-riscv-book/docs/CourseOutline.html  
评价标准：https://enzoding-rgb.github.io/ruyi-riscv-book/course-evaluation-standard.md  
ch03 讲义：https://enzoding-rgb.github.io/ruyi-riscv-book/chapters/ch03/lecture.html  
ch03 实验：https://enzoding-rgb.github.io/ruyi-riscv-book/chapters/ch03/lab.html  
ch04 讲义：https://enzoding-rgb.github.io/ruyi-riscv-book/chapters/ch04/lecture.html  
ch04 实验：https://enzoding-rgb.github.io/ruyi-riscv-book/chapters/ch04/lab.html

## 仓库结构

```text
ruyi-riscv-linux-book/
├── README.md
├── docs/
│   ├── CourseOutline.html
│   ├── course-evaluation-standard.md
│   ├── intro.md
│   └── index.html
├── chapters/
│   └── ch01/ … ch06/
├── reference/
└── misc/
    ├── scripts/
    ├── boards/
    └── archive/
```

## 当前进度

- **CourseOutline / ch01–ch06 讲义与实验：** 已按定稿上线 Pages  
- **课程评价标准：** `docs/course-evaluation-standard.md` 已就绪  

制作分支：`enzo`。
