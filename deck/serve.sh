#!/bin/sh
ROOT="$(cd "$(dirname "$0")/.." && pwd)"
echo "Deck: http://127.0.0.1:8765/deck/"
echo "Arrow keys to change slides. ESC for overview."
cd "$ROOT" || exit 1
exec python3 -m http.server 8765
