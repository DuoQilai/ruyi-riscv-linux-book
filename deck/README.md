# 组内课程介绍 Deck

**分支：** `design/internal-deck`  
**用途：** 组内汇报。请不要把这个分支开成 PR，也不要合入 `enzo` / `main`。

10 页。第 3 页嵌入第一章讲义、第四章实验。

课程主页：https://enzoding-rgb.github.io/ruyi-riscv-book/

## 怎么放

```bash
fuser -k 8765/tcp 2>/dev/null; cd /home/fengde/Projects/full-stack/ruyi/ruyi-riscv-linux-book/deck && python3 serve.py &
```

打开 http://127.0.0.1:8765/#1 （serve.py 强制 no-store，浏览器永不缓存；右下角徽标显示 git + 服务器时间，确认没看旧版。首次切到 serve.py 后硬刷新一次即可）

- **← / ↑ / PageUp**：上一页
- **→ / ↓ / PageDown**：下一页
- **ESC**：回到概览墙
- 空格留给录像播放

## 录像位（演示页 08-demo）

- `deck/videos/ch03-thermo.mp4`（模拟温度 · 风扇）
- `deck/videos/ch04-thermo.mp4`（真 DHT22 · 终端命令）

视频已在 `.gitignore`。MP4/H.264，Chrome 倍速插件可直接生效。
