# 组内课程介绍 Deck

**分支：** `design/internal-deck`  
**用途：** 组内汇报。请不要把这个分支开成 PR，也不要合入 `enzo` / `main`。

9 页 HTML 幻灯片，浏览器全屏，方向键翻页。

课程主页：https://enzoding-rgb.github.io/ruyi-riscv-book/

## 怎么放

```bash
python3 -m http.server 8765 --directory /home/fengde/Projects/full-stack/ruyi/ruyi-riscv-linux-book/deck &
```

打开 http://127.0.0.1:8765/

- **← / ↑ / PageUp**：上一页
- **→ / ↓ / PageDown**：下一页
- **ESC**：回到概览墙
- 空格留给录像播放

## 录像位（架构页）

第三、四章用**一段**完整录像，放到：

- `deck/videos/thermo-demo.mp4`

mp4 已写进 `.gitignore`。
