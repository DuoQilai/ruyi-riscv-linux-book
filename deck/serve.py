#!/usr/bin/env python3
"""Deck server: 强制 no-store 防缓存 + /__version 让页面证明自己是最新的。

用法：python3 serve.py   （README 里那条命令已改用它）
"""
import json
import os
import subprocess
import time
from http.server import SimpleHTTPRequestHandler, ThreadingHTTPServer

HERE = os.path.dirname(os.path.abspath(__file__))
PORT = 8765


class Handler(SimpleHTTPRequestHandler):
    def __init__(self, *args, **kwargs):
        super().__init__(*args, directory=HERE, **kwargs)

    def end_headers(self):
        # 所有 deck 文件都不许缓存：浏览器每次刷新都命中磁盘上的最新内容
        self.send_header("Cache-Control", "no-store, max-age=0")
        super().end_headers()

    def do_GET(self):
        if self.path.split("?")[0] == "/__version":
            self._version()
            return
        super().do_GET()

    def _version(self):
        data = {
            "time": time.strftime("%H:%M:%S"),
            "git": self._git("rev-parse", "--short", "HEAD"),
            "last_commit": self._git("log", "-1", "--format=%cd", "--date=format:%m-%d %H:%M"),
        }
        body = json.dumps(data, ensure_ascii=False).encode()
        self.send_response(200)
        self.send_header("Content-Type", "application/json; charset=utf-8")
        self.send_header("Content-Length", str(len(body)))
        self.end_headers()
        self.wfile.write(body)

    def _git(self, *args):
        try:
            return subprocess.check_output(
                ["git", "-C", HERE] + list(args), stderr=subprocess.DEVNULL
            ).decode().strip()
        except Exception:
            return ""


if __name__ == "__main__":
    print("Deck: http://127.0.0.1:8765/  (no-store; /__version 见 git + 时间)")
    ThreadingHTTPServer(("0.0.0.0", PORT), Handler).serve_forever()
