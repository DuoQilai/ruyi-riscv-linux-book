#!/bin/bash
# deploy-pages.sh — 把课程内容同步到 Enzo Ding 个人 GitHub Pages
# 用法: bash scripts/deploy-pages.sh

set -e

PAGES_REPO="https://github.com/EnzoDing-rgb/EnzoDing-rgb.github.io.git"
PAGES_DIR="/tmp/enzoding-pages-deploy"
COURSE_PATH="ruyi-riscv-book"

echo "==> Cloning Pages repo..."
rm -rf "$PAGES_DIR"
git clone --depth 1 "$PAGES_REPO" "$PAGES_DIR"

echo "==> Syncing course content..."
DST="$PAGES_DIR/$COURSE_PATH"
mkdir -p "$DST"/{chapters/ch01,chapters/ch02}

# CourseOutline (at root level for easy access)
cp docs/CourseOutline.html "$DST/CourseOutline.html"

# ch01
cp chapters/ch01/lecture.html "$DST/chapters/ch01/"
cp chapters/ch01/lecture.pdf  "$DST/chapters/ch01/"
cp chapters/ch01/lab.html     "$DST/chapters/ch01/"
cp chapters/ch01/lab.pdf      "$DST/chapters/ch01/"

# ch02
cp chapters/ch02/lecture.html "$DST/chapters/ch02/"
cp chapters/ch02/lecture.pdf  "$DST/chapters/ch02/"
cp chapters/ch02/lab.html     "$DST/chapters/ch02/"
cp chapters/ch02/lab.pdf      "$DST/chapters/ch02/"
cp -r chapters/ch02/code      "$DST/chapters/ch02/"

echo "==> Committing and pushing..."
cd "$PAGES_DIR"
git add -A
git commit -m "deploy: sync course content $(date +%Y-%m-%d)" || echo "(no changes to commit)"
git push origin main

echo "==> Done. Live at: https://enzoding-rgb.github.io/$COURSE_PATH/"
rm -rf "$PAGES_DIR"
