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
git add index.html docs/CourseOutline.html docs/index.html chapters/ misc/scripts/inject_course_nav.py misc/scripts/build.sh
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
cp docs/index.html "$DST/docs/index.html"
# 旧书签 /CourseOutline.html → 跳到 docs/（避免根路径下 ../chapters 链失效）
cp CourseOutline.html "$DST/CourseOutline.html"
# GitHub Pages / Jekyll：避免偶发忽略路径
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
echo "  入口(总览):    $BASE/"
echo "  CourseOutline: $BASE/docs/CourseOutline.html"
echo "  (兼容旧链):    $BASE/CourseOutline.html"
for ch in ch01 ch02 ch03 ch04 ch05 ch06; do
  echo "  $ch lecture:  $BASE/chapters/$ch/lecture.html"
  echo "  $ch lab:      $BASE/chapters/$ch/lab.html"
done
