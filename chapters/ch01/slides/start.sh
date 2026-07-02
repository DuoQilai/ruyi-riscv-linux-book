#!/bin/bash
cd "$(dirname "$0")"
PORT=${1:-8899}
echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
echo "  第一章 开发环境篇 · RISC-V Linux 课程"
echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
echo "  Deck:  http://localhost:$PORT"
echo "  按 P 键 → 浏览器打印为 PDF"
echo "  VS Code / Cursor: 自动端口转发 Ctrl+Shift+P → Forward Port"
echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
python3 -m http.server "$PORT"
