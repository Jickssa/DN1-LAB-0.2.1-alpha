#!/usr/bin/env python3
"""Validate the six host-only prototype manifests and catalogue."""
from __future__ import annotations
import json
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
EXPECTED = {
    "VESPER": (103, 60),
    "MYCEL": (104, 40),
    "ORBITAL": (105, 44),
    "LITHOS": (106, 44),
    "PHOTON": (107, 40),
    "ANIMA": (108, 56),
}

def load(path: Path):
    try:
        return json.loads(path.read_text(encoding="utf-8"))
    except (OSError, json.JSONDecodeError) as exc:
        raise SystemExit(f"ERROR {path.relative_to(ROOT)}: {exc}") from exc

seen_ids: set[int] = set()
entries = {}
for name, (expected_id, expected_state) in EXPECTED.items():
    path = ROOT / "prototypes" / name.lower() / "prototype.json"
    item = load(path)
    entries[name] = item
    assert item.get("schema_version") == 1, f"{name}: unsupported schema"
    assert item.get("name") == name, f"{name}: name mismatch"
    assert item.get("id") == expected_id, f"{name}: ID mismatch"
    assert expected_id not in seen_ids, f"{name}: duplicate ID"
    seen_ids.add(expected_id)
    assert item.get("id_status") == "provisional-not-linker-validated", f"{name}: ID must remain provisional"
    assert item.get("status") == "host-prototype", f"{name}: wrong status"
    assert item.get("buildable") is False, f"{name}: must not claim a firmware build"
    assert item.get("hardware_certified") is False, f"{name}: must not claim hardware certification"
    assert item.get("state_bytes_host") == expected_state, f"{name}: state-size declaration mismatch"
    assert 0 < expected_state <= 256, f"{name}: state exceeds SDK state ceiling"
    controls = item.get("controls", [])
    assert len(controls) == 15, f"{name}: expected 15 controls"
    assert len(set(controls)) == 15, f"{name}: duplicate control label"
    assert all(isinstance(c, str) and 1 <= len(c) <= 5 for c in controls), f"{name}: invalid encoder label"
    pages = item.get("pages", [])
    assert len(pages) == 2, f"{name}: expected two pages"
    assert all(isinstance(p, str) and 1 <= len(p) <= 16 for p in pages), f"{name}: invalid page label"
    target = ROOT / item["source"]
    assert target.is_file(), f"{name}: missing source {item['source']}"
    preview = item.get("audio_preview")
    if preview and not (ROOT / preview).is_file():
        print(f"INFO {name}: preview is supplied in the source archive, not this repository checkout")

catalogue = load(ROOT / "prototypes" / "catalogue.json")
assert catalogue.get("version") == "0.3.0-alpha"
assert catalogue.get("status") == "host-prototypes-only"
rows = catalogue.get("machines", [])
assert len(rows) == len(EXPECTED), "catalogue must contain exactly six prototypes"
assert {x.get("name") for x in rows} == set(EXPECTED), "catalogue/name set mismatch"
for row in rows:
    name = row["name"]
    item = entries[name]
    assert row.get("id") == item["id"], f"{name}: catalogue ID mismatch"
    assert row.get("state_bytes_host") == item["state_bytes_host"], f"{name}: catalogue state mismatch"
    assert row.get("controls") == 15, f"{name}: catalogue control count mismatch"
    assert row.get("pages") == item["pages"], f"{name}: catalogue page mismatch"
    assert row.get("id_status") == "provisional", f"{name}: catalogue ID status mismatch"
print("PASS prototype manifests: six unique provisional IDs, 15 controls, two pages, valid paths, <=256-byte states")