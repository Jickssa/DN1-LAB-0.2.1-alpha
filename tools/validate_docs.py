#!/usr/bin/env python3
"""Validate relative Markdown links in tracked project documentation."""
from __future__ import annotations
import re
from pathlib import Path
from urllib.parse import unquote, urlparse

ROOT = Path(__file__).resolve().parents[1]
LINK = re.compile(r'(?<!!)\[[^\]]*\]\(([^)]+)\)')
IGNORE_DIRS = {".git", ".venv", "__pycache__"}

def main() -> int:
    errors: list[str] = []
    files = sorted(
        p for p in ROOT.rglob("*.md")
        if not any(part in IGNORE_DIRS for part in p.relative_to(ROOT).parts)
    )
    checked = 0
    for source in files:
        text = source.read_text(encoding="utf-8")
        for raw in LINK.findall(text):
            target = raw.strip().split()[0].strip("<>")
            parsed = urlparse(target)
            if parsed.scheme or target.startswith(("//", "#")):
                continue
            path = unquote(parsed.path.split("#", 1)[0].split("?", 1)[0])
            if not path:
                continue
            candidate = (source.parent / path).resolve()
            if not candidate.exists():
                errors.append(f"{source.relative_to(ROOT)} -> {target}")
            checked += 1
    if errors:
        for item in errors:
            print(f"FAIL markdown link: {item}")
        return 1
    print(f"PASS Markdown links: {len(files)} files, {checked} relative links")
    return 0

if __name__ == "__main__":
    raise SystemExit(main())
