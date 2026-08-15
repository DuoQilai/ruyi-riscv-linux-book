# 组内课程介绍 Deck

**分支：** `design/internal-deck`  
**用途：** 组内汇报。请不要把这个分支开成 PR，也不要合入 `enzo` / `main`。

10 页。第 3 页嵌入第一章讲义、第四章实验，让人看见学生打开的材料。

课程主页：https://enzoding-rgb.github.io/ruyi-riscv-book/

## 怎么放

服务要起在**仓库根**，举例页才能加载 `chapters/`：

```bash
python3 -m http.server 8765 --directory /home/fengde/Projects/full-stack/ruyi/ruyi-riscv-linux-book &
```

打开 http://127.0.0.1:8765/deck/

- **← / ↑ / PageUp**：上一页
- **→ / ↓ / PageDown**：下一页
- **ESC**：回到概览墙
- 空格留给录像播放

## 录像位（架构页）

- `deck/videos/thermo-demo.mp4`

mp4 已写进 `.gitignore`。
