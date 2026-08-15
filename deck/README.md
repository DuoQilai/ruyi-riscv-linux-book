# 组内课程介绍 Deck

**分支：** `design/internal-deck`  
**用途：** 组内汇报。请不要把这个分支开成 PR，也不要合入 `enzo` / `main`。

10 页。第 3 页嵌入第一章讲义、第四章实验。

课程主页：https://enzoding-rgb.github.io/ruyi-riscv-book/

## 怎么放

```bash
fuser -k 8765/tcp 2>/dev/null; python3 -m http.server 8765 --directory /home/fengde/Projects/full-stack/ruyi/ruyi-riscv-linux-book/deck &
```

打开 http://127.0.0.1:8765/#1 （硬刷新一次，清掉旧的 8 页缓存）

- **← / ↑ / PageUp**：上一页
- **→ / ↓ / PageDown**：下一页
- **ESC**：回到概览墙
- 空格留给录像播放

## 录像位（架构页）

- `deck/videos/thermo-demo.mp4`
