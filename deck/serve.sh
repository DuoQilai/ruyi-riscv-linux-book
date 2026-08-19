#!/bin/sh
HERE="$(cd "$(dirname "$0")" && pwd)"
fuser -k 8765/tcp 2>/dev/null
echo "Deck: http://127.0.0.1:8765/"
cd "$HERE" || exit 1
exec python3 serve.py
