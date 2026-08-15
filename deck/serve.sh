#!/bin/sh
ROOT="$(cd "$(dirname "$0")/.." && pwd)"
fuser -k 8765/tcp 2>/dev/null
echo "Deck: http://127.0.0.1:8765/deck/"
cd "$ROOT" || exit 1
exec python3 -m http.server 8765
