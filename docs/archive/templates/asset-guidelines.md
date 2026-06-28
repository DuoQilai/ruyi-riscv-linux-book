# 图片与图表规则

## 存放位置

每章图片、截图和图表统一放在本章的 `assets/` 目录。图片内容必须服务于课程主线，截图应来自实际验证环境或明确标注为示意图。

```text
chapters/chXX_topic/
├── assets/
├── 1.1.md              # 节讲义（ch02 为 2.1.md 等）
├── …
└── lab/
    └── 1.1.md          # 实验（ch02 为 2.1.md 等）
```

## 命名规则

建议使用：

```text
fig-章节号-序号-说明.png
```

示例：

```text
fig-01-01-ruyi-version.png
fig-02-04-fan-wiring.png
fig-04-04-mqtt-light-status.png
fig-06-02-detection-result.png
```

## 引用规则

正文中引用图片时，必须有编号和解释：

```text
图片编号：图 1-1
图片标题：ruyi version 输出
图片路径：assets/fig-01-01-ruyi-version.png

图 1-1 展示了 `ruyi version` 的输出结果，用于确认 `ruyi` 已正确安装。
```
