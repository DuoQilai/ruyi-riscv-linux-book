#!/bin/bash
# build.sh — PDF 生成 → 提交课程仓库 → 部署 GitHub Pages
# 用法: bash scripts/build.sh [commit message]

set -e

MSG="${1:-update course content $(date +%Y-%m-%d)}"
ROOT="$(cd "$(dirname "$0")/.." && pwd)"
PAGES_REPO="https://github.com/EnzoDing-rgb/EnzoDing-rgb.github.io.git"
COURSE_PATH="ruyi-riscv-book"

cd "$ROOT"

echo "━━━ 1/3 生成 PDF ━━━"
PDF_LIST=(
  "$ROOT/chapters/ch01/lecture.html"
  "$ROOT/chapters/ch01/lab.html"
  "$ROOT/chapters/ch02/lecture.html"
  "$ROOT/chapters/ch02/lab.html"
)
for f in "${PDF_LIST[@]}"; do
  node -e "
const { chromium } = require('playwright');
const fs = require('fs');
const f = process.argv[1];
(async () => {
  const browser = await chromium.launch();
  const ctx = await browser.newContext({ viewport: { width: 1200, height: 1600 } });
  const page = await ctx.newPage();
  await page.goto('file://' + f, { waitUntil: 'networkidle' });
  await page.emulateMedia({ media: 'print' });
  await page.waitForTimeout(1000);
  const out = f.replace('.html','.pdf');
  await page.pdf({ path: out, format: 'A4', printBackground: true, margin: { top:'15mm',right:'15mm',bottom:'15mm',left:'25mm' } });
  console.log('  ✓ ' + out.split('/').pop());
  await ctx.close();
  await browser.close();
})();
" "$f"
done

echo ""
echo "━━━ 2/3 提交课程仓库 ━━━"
git add -A
if git diff --cached --quiet; then
  echo "  (no changes)"
else
  git commit -m "$MSG"
  git push origin enzo
  echo "  ✓ pushed to enzo"
fi

echo ""
echo "━━━ 3/3 部署 GitHub Pages ━━━"
PAGES_DIR=$(mktemp -d)
git clone --depth 1 "$PAGES_REPO" "$PAGES_DIR" 2>/dev/null
DST="$PAGES_DIR/$COURSE_PATH"
mkdir -p "$DST"/{chapters/ch01,chapters/ch02}

cp docs/CourseOutline.html "$DST/CourseOutline.html"
cp chapters/ch01/lecture.html "$DST/chapters/ch01/"
cp chapters/ch01/lecture.pdf  "$DST/chapters/ch01/"
cp chapters/ch01/lab.html     "$DST/chapters/ch01/"
cp chapters/ch01/lab.pdf      "$DST/chapters/ch01/"
cp chapters/ch02/lecture.html "$DST/chapters/ch02/"
cp chapters/ch02/lecture.pdf  "$DST/chapters/ch02/"
cp chapters/ch02/lab.html     "$DST/chapters/ch02/"
cp chapters/ch02/lab.pdf      "$DST/chapters/ch02/"
cp -r chapters/ch02/code      "$DST/chapters/ch02/" 2>/dev/null || true

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
echo "  CourseOutline: https://enzoding-rgb.github.io/$COURSE_PATH/CourseOutline.html"
echo "  ch01 lecture:  https://enzoding-rgb.github.io/$COURSE_PATH/chapters/ch01/lecture.html"
echo "  ch01 lab:      https://enzoding-rgb.github.io/$COURSE_PATH/chapters/ch01/lab.html"
echo "  ch02 lecture:  https://enzoding-rgb.github.io/$COURSE_PATH/chapters/ch02/lecture.html"
echo "  ch02 lab:      https://enzoding-rgb.github.io/$COURSE_PATH/chapters/ch02/lab.html"
