#!/usr/bin/env python3
"""Validate DN1-LAB catalogue entries against SDK-alpha descriptors and C IDs."""
from __future__ import annotations
import json
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
EXPECTED = {
    "karst": (100, "KARST", "dn1-lab-karst"),
    "morphant": (101, "MORPHANT", "dn1-lab-morphant"),
    "tessera": (102, "TESSERA", "dn1-lab-tessera"),
}


def fail(message: str) -> None:
    raise SystemExit(f"FAIL catalogue: {message}")


def read_c_machine_id(source: str, slug: str) -> int:
    candidates = [
        line for line in source.splitlines()
        if ".id" in line and "=" in line
    ]
    if len(candidates) != 1:
        fail(f"{slug}: expected exactly one C machine .id assignment, found {len(candidates)}")
    rhs = candidates[0].split(".id", 1)[1].split("=", 1)[1]
    token = rhs.strip().split(",", 1)[0].strip()
    if not token.isascii() or not token.isdecimal():
        fail(f"{slug}: C machine ID is not a decimal integer: {token!r}")
    return int(token)


def main() -> int:
    ids: set[int] = set()
    for slug, (expected_id, expected_name, package_id) in EXPECTED.items():
        path = ROOT / "catalogue" / "machines" / f"{slug}.json"
        if not path.is_file():
            fail(f"missing {path.relative_to(ROOT)}")
        try:
            item = json.loads(path.read_text(encoding="utf-8"))
        except (OSError, json.JSONDecodeError) as exc:
            fail(f"invalid JSON in {path}: {exc}")
        required = {
            "schema_version", "slug", "name", "machine_id", "package_id",
            "version", "status", "target", "control_count", "custom_pages",
            "machine_state_bytes_max", "license", "source", "validation",
            "capabilities",
        }
        missing = required - item.keys()
        if missing:
            fail(f"{slug}: missing fields {sorted(missing)}")
        if item["schema_version"] != 1 or item["slug"] != slug:
            fail(f"{slug}: schema version or slug mismatch")
        if item["machine_id"] != expected_id or item["name"].split(" — ")[0] != expected_name:
            fail(f"{slug}: catalogue ID/name mismatch")
        if expected_id in ids or not 4 <= expected_id <= 127:
            fail(f"{slug}: duplicate/out-of-range ID")
        ids.add(expected_id)
        if item["package_id"] != package_id or item["version"] != "0.2.2-alpha":
            fail(f"{slug}: SDK package identity/version mismatch")
        if item["license"] != "GPL-2.0-or-later":
            fail(f"{slug}: licence mismatch")
        if item["target"].get("os") != ["1.44"] or not any(
            "Digitone MKI" in x for x in item["target"].get("hardware", [])
        ):
            fail(f"{slug}: target must include Digitone MKI OS 1.44")
        if (
            item["control_count"] != 15
            or item["custom_pages"] != 2
            or item["machine_state_bytes_max"] != 256
        ):
            fail(f"{slug}: SDK resource contract mismatch")
        validation = item["validation"]
        if (
            validation.get("level") != "L2"
            or validation.get("target_sdk_build") is not False
            or validation.get("emulator") is not False
            or validation.get("hardware") is not False
        ):
            fail(f"{slug}: unsupported validation level/claim")
        sdk_path = ROOT / "sdk-alpha" / slug / "mod.json"
        c_path = ROOT / "sdk-alpha" / slug / "machine.c"
        try:
            sdk = json.loads(sdk_path.read_text(encoding="utf-8"))
            code = c_path.read_text(encoding="utf-8")
        except (OSError, json.JSONDecodeError) as exc:
            fail(f"{slug}: invalid SDK source/manifest: {exc}")
        if sdk.get("id") != package_id or sdk.get("version") != item["version"]:
            fail(f"{slug}: SDK descriptor identity/version mismatch")
        if sdk.get("devices") != {"digitone-mk1": ["1.44"]}:
            fail(f"{slug}: SDK device matrix mismatch")
        if read_c_machine_id(code, slug) != expected_id:
            fail(f"{slug}: C source machine ID mismatch")
        if item["source"] != f"sdk-alpha/{slug}/":
            fail(f"{slug}: source path mismatch")
        print(
            f"PASS catalogue {slug}: id={expected_id}, "
            f"version={item['version']}, target OS 1.44, evidence L2"
        )
    proposal_dir = ROOT / "catalogue" / "proposals"
    proposals = sorted(proposal_dir.glob("*.md"))
    if len(proposals) != 6:
        fail(f"expected six research proposals, found {len(proposals)}")
    print("PASS catalogue: three machine entries, unique IDs, and six proposals")
    return 0


if __name__ == "__main__":
    sys.exit(main())
