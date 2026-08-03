#!/bin/bash
# build.sh — 提交课程仓库 → 部署 GitHub Pages
# 用法: bash misc/scripts/build.sh [commit message]

set -e

MSG="${1:-update course content $(date +%Y-%m-%d)}"
ROOT="$(cd "$(dirname "$0")/../.." && pwd)"
PAGES_REPO="https://github.com/EnzoDing-rgb/EnzoDing-rgb.github.io.git"
COURSE_PATH="ruyi-riscv-book"

cd "$ROOT"

echo "━━━ 1/2 提交课程仓库 ━━━"
git add -A index.html docs/CourseOutline.html chapters/ misc/scripts/inject_course_nav.py misc/scripts/build.sh
# 去掉已废弃的跳转页（若仍在索引里）
git rm -f --ignore-unmatch CourseOutline.html docs/index.html
if git diff --cached --quiet; then
  echo "  (no changes)"
else
  git commit -m "$MSG"
  git push origin enzo
  echo "  ✓ pushed to enzo"
fi

echo ""
echo "━━━ 2/2 部署 GitHub Pages ━━━"
PAGES_DIR=$(mktemp -d)
git clone --depth 1 "$PAGES_REPO" "$PAGES_DIR" 2>/dev/null
DST="$PAGES_DIR/$COURSE_PATH"
mkdir -p "$DST"/chapters/ch0{1,2,3,4,5,6}
mkdir -p "$DST/docs"

cp index.html "$DST/index.html"
cp docs/CourseOutline.html "$DST/docs/CourseOutline.html"
# 不再部署任何「兼容跳转」页；并删掉站点上遗留的坏链文件
rm -f "$DST/CourseOutline.html" "$DST/docs/index.html"
touch "$DST/.nojekyll"

for ch in ch01 ch02 ch03 ch04 ch05 ch06; do
  cp "chapters/$ch/lecture.html" "$DST/chapters/$ch/"
  cp "chapters/$ch/lab.html"     "$DST/chapters/$ch/"
  cp -r "chapters/$ch/code"      "$DST/chapters/$ch/" 2>/dev/null || true
done

cd "$PAGES_DIR"
git add -A
if git diff --cached --quiet; then
  echo "  (no changes)"
else
  git commit -m "$MSG"
  git push origin main
  echo "  ✓ deployed to Pages"
fi
rm -rf "$PAGES_DIR"

echo ""
echo "━━━ Done ━━━"
BASE="https://enzoding-rgb.github.io/$COURSE_PATH"
echo "  课程主页:      $BASE/"
echo "  CourseOutline: $BASE/docs/CourseOutline.html"
for ch in ch01 ch02 ch03 ch04 ch05 ch06; do
  echo "  $ch lecture:  $BASE/chapters/$ch/lecture.html"
  echo "  $ch lab:      $BASE/chapters/$ch/lab.html"
done
