# 组内课程介绍 Deck

**分支：** `design/internal-deck`  
**用途：** 组内汇报。请不要把这个分支开成 PR，也不要合入 `enzo` / `main`。

8 页 HTML 幻灯片，浏览器全屏，方向键翻页。

## 怎么放

多页用 iframe 拼起来，直接 `file://` 打开会拦跨文件加载。用一个静态 HTTP 服务：

```bash
cd deck
python3 -m http.server 8765
```

或：

```bash
bash deck/serve.sh
```

浏览器打开 http://127.0.0.1:8765/

- **← / ↑ / PageUp**：上一页
- **→ / ↓ / PageDown**：下一页
- **ESC**：回到概览墙；点任意页再进演示
- 空格留给录像播放，不用来翻页

## 录像位（第 5 页）

把文件放到这里就会自动替换灰框：

- `deck/videos/ch03-thermo.mp4` — 第三章温控
- `deck/videos/ch04-thermo.mp4` — 第四章命令温控

mp4 已写进 `.gitignore`，录像本身不进 git。
