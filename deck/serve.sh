#!/bin/sh
cd "$(dirname "$0")"
echo "Deck: http://127.0.0.1:8765/"
echo "Arrow keys to change slides. ESC for overview."
exec python3 -m http.server 8765
