#!/usr/bin/env python3
"""Check local links and assets in the static DN1-LAB site."""
from __future__ import annotations
from html.parser import HTMLParser
from pathlib import Path
from urllib.parse import urlparse
import sys

ROOT = Path(__file__).resolve().parents[1]
SITE = ROOT / "site"

class Refs(HTMLParser):
    def __init__(self) -> None:
        super().__init__()
        self.refs: list[str] = []
    def handle_starttag(self, tag: str, attrs: list[tuple[str, str | None]]) -> None:
        values = dict(attrs)
        for key in ("href", "src"):
            value = values.get(key)
            if value:
                self.refs.append(value)

def main() -> int:
    errors: list[str] = []
    pages = sorted(SITE.rglob("*.html"))
    if not pages:
        raise SystemExit("FAIL site: no HTML pages found")
    for page in pages:
        parser = Refs()
        parser.feed(page.read_text(encoding="utf-8"))
        for ref in parser.refs:
            parsed = urlparse(ref)
            if parsed.scheme or ref.startswith(("#", "mailto:", "tel:")):
                continue
            target = (page.parent / parsed.path).resolve()
            if target.is_dir():
                target = target / "index.html"
            if not target.exists():
                errors.append(f"{page.relative_to(ROOT)} -> {ref}")
    if errors:
        for error in errors:
            print("FAIL local site reference:", error)
        return 1
    print(f"PASS site: {len(pages)} HTML pages; all local href/src targets exist")
    return 0

if __name__ == "__main__":
    sys.exit(main())
