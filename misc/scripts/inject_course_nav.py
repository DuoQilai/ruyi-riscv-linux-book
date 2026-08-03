#!/usr/bin/env python3
"""Inject course hub / prev / next nav into chapter lecture & lab HTML."""
from __future__ import annotations

from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
CH = ROOT / "chapters"

# Linear reading order for bottom "上一篇 / 下一篇"
ORDER = [
    ("ch01", "lecture", "第一章 · 讲义", "环境与工具链"),
    ("ch01", "lab", "第一章 · 实验", "上板通道验收"),
    ("ch02", "lecture", "第二章 · 讲义", "够用的 C"),
    ("ch02", "lab", "第二章 · 实验", "改代码闯关"),
    ("ch03", "lecture", "第三章 · 讲义", "GPIO 与执行器"),
    ("ch03", "lab", "第三章 · 实验", "温控风扇"),
    ("ch04", "lecture", "第四章 · 讲义", "串口对话与温控"),
    ("ch04", "lab", "第四章 · 实验", "串口命令温控"),
    ("ch05", "lecture", "第五章 · 讲义", "网络与 MQTT"),
    ("ch05", "lab", "第五章 · 实验", "MQTT 远程控灯"),
    ("ch06", "lecture", "第六章 · 讲义", "线程与协同"),
    ("ch06", "lab", "第六章 · 实验", "三线程协同"),
]

NAV_CSS = """
  .course-rail a { font-weight: 600; color: var(--accent, #2e6b6e); }
  .page-nav {
    display: flex; flex-wrap: wrap; justify-content: space-between; gap: 16px;
    margin: 48px 0 24px; padding: 24px 0 8px;
    border-top: 1px solid var(--line, #d4cfc4);
    font-size: 16px;
  }
  .page-nav a {
    color: var(--accent, #2e6b6e); text-decoration: none; max-width: 46%;
    line-height: 1.45;
  }
  .page-nav a:hover { text-decoration: underline; }
  .page-nav .dir { display: block; font-size: 13px; color: var(--faint, #8c8276); margin-bottom: 4px; font-weight: 400; }
  .page-nav .next { text-align: right; margin-left: auto; }
"""

MARKER_CSS = "/* course-nav-css */"
MARKER_TOC = "<!-- course-nav-toc -->"
MARKER_FOOT = "<!-- course-nav-foot -->"


def sibling(ch: str, kind: str) -> tuple[str, str]:
    other = "lab" if kind == "lecture" else "lecture"
    label = "本章实验" if other == "lab" else "本章讲义"
    return f"{other}.html", label


def build_toc_block(ch: str, kind: str, idx: int) -> str:
    sib_href, sib_label = sibling(ch, kind)
    lines = [
        MARKER_TOC,
        '  <a class="course-rail" href="../../index.html">课程总览</a>',
        '  <a class="course-rail" href="../../docs/CourseOutline.html">Course Outline</a>',
        f'  <a class="course-rail" href="{sib_href}">{sib_label}</a>',
    ]
    if idx + 1 < len(ORDER):
        nch, nkind, ntitle, _ = ORDER[idx + 1]
        if nch != ch:
            href = f"../{nch}/{nkind}.html"
            lines.append(
                f'  <a class="course-rail" href="{href}">下一章 · {ntitle.split("·")[0].strip()}</a>'
            )
    lines.append('  <div class="divider"></div>')
    return "\n".join(lines) + "\n"


def build_foot(idx: int) -> str:
    prev_html = ""
    next_html = ""
    if idx > 0:
        pch, pkind, ptitle, psub = ORDER[idx - 1]
        ch, kind, _, _ = ORDER[idx]
        href = f"{pkind}.html" if pch == ch else f"../{pch}/{pkind}.html"
        if idx == 0:
            pass
        prev_html = (
            f'<a class="prev" href="{href}">'
            f'<span class="dir">上一篇</span>{ptitle}<br>{psub}</a>'
        )
    else:
        prev_html = (
            '<a class="prev" href="../../index.html">'
            '<span class="dir">上一篇</span>课程总览</a>'
        )

    if idx + 1 < len(ORDER):
        nch, nkind, ntitle, nsub = ORDER[idx + 1]
        ch, kind, _, _ = ORDER[idx]
        href = f"{nkind}.html" if nch == ch else f"../{nch}/{nkind}.html"
        next_html = (
            f'<a class="next" href="{href}">'
            f'<span class="dir">下一篇</span>{ntitle}<br>{nsub}</a>'
        )
    else:
        next_html = (
            '<a class="next" href="../../docs/CourseOutline.html">'
            '<span class="dir">下一篇</span>综合项目 · 见 Course Outline</a>'
        )

    # fix prev for idx>0 — rewrite cleanly
    if idx > 0:
        pch, pkind, ptitle, psub = ORDER[idx - 1]
        ch = ORDER[idx][0]
        href = f"{pkind}.html" if pch == ch else f"../{pch}/{pkind}.html"
        prev_html = (
            f'<a class="prev" href="{href}">'
            f'<span class="dir">上一篇</span>{ptitle}<br>{psub}</a>'
        )

    return (
        f"{MARKER_FOOT}\n"
        f'<nav class="page-nav" aria-label="章节翻页">\n'
        f"  {prev_html}\n"
        f"  {next_html}\n"
        f"</nav>\n"
    )


def strip_old(text: str) -> str:
    # Remove previous injection blocks if re-run
    while MARKER_CSS in text:
        start = text.find(MARKER_CSS)
        # remove from marker through closing of that style rule block we added — simpler: regenerate file from git? 
        # For idempotent: remove between markers
        end = text.find("*/", start)
        if end < 0:
            break
        # find end of NAV_CSS insertion: after `*/` of marker comment we have NAV_CSS then nothing special
        # Actually we insert: MARKER_CSS + "\n" + NAV_CSS
        # Remove from MARKER_CSS to end of NAV_CSS (last rule)
        css_end = text.find(".page-nav .next { text-align: right; margin-left: auto; }\n", start)
        if css_end < 0:
            break
        css_end = text.find("\n", css_end + 1) + 1
        text = text[:start] + text[css_end:]
    while MARKER_TOC in text:
        start = text.find(MARKER_TOC)
        end = text.find('<div class="divider"></div>\n', start)
        if end < 0:
            break
        end = text.find("\n", end) + 1
        # may have been our divider right after toc block — remove through first divider after marker
        text = text[:start] + text[end:]
    while MARKER_FOOT in text:
        start = text.find(MARKER_FOOT)
        end = text.find("</nav>", start)
        if end < 0:
            break
        end = text.find("\n", end) + 1
        text = text[:start] + text[end:]
    return text


def inject(path: Path, idx: int, ch: str, kind: str) -> None:
    text = path.read_text(encoding="utf-8")
    text = strip_old(text)

    if MARKER_CSS not in text:
        if "</style>" not in text:
            raise SystemExit(f"no </style> in {path}")
        text = text.replace(
            "</style>",
            f"{MARKER_CSS}\n{NAV_CSS}</style>",
            1,
        )

    # TOC block after <nav class="toc"> opening
    if MARKER_TOC not in text:
        needle = '<nav class="toc">'
        pos = text.find(needle)
        if pos < 0:
            raise SystemExit(f"no toc nav in {path}")
        insert_at = pos + len(needle)
        # skip newline
        if text[insert_at : insert_at + 1] == "\n":
            insert_at += 1
        # after <h4>...</h4>\n
        h4_end = text.find("</h4>", insert_at)
        if h4_end > 0:
            insert_at = text.find("\n", h4_end) + 1
        block = build_toc_block(ch, kind, idx)
        text = text[:insert_at] + block + text[insert_at:]

    if MARKER_FOOT not in text:
        foot = build_foot(idx)
        if "</article>" in text:
            text = text.replace("</article>", foot + "</article>", 1)
        elif "</div>\n</body>" in text:
            text = text.replace("</div>\n</body>", foot + "</div>\n</body>", 1)
        else:
            text = text.replace("</body>", foot + "</body>", 1)

    path.write_text(text, encoding="utf-8")
    print(f"updated {path.relative_to(ROOT)}")


def main() -> None:
    for idx, (ch, kind, _, _) in enumerate(ORDER):
        path = CH / ch / f"{kind}.html"
        if not path.exists():
            raise SystemExit(f"missing {path}")
        inject(path, idx, ch, kind)


if __name__ == "__main__":
    main()
