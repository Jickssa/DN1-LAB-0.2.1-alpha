#!/bin/sh
# Reproducible source-to-image pipeline for DN1-LAB 0.2.2-alpha.
# Requires the exact upstream shared SDK alpha, DN1 OS1.44 companions and ColdFire toolchain.
set -eu

ROOT=$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)
: "${ELEKLOADER_ROOT:?Set ELEKLOADER_ROOT to a local elekloader checkout}"
: "${STOCK:?Set STOCK to the official Digitone MKI OS 1.44 SysEx file}"

[ -f "$STOCK" ] || { echo "ERROR: stock firmware not found: $STOCK" >&2; exit 2; }
[ -f "$ELEKLOADER_ROOT/elekloader/sdk/sdk_alpha_build.py" ] || {
  echo "ERROR: shared SDK-alpha builder not found in upstream checkout" >&2; exit 2;
}
[ -f "$ELEKLOADER_ROOT/mods/core-dn1/mod.json" ] || {
  echo "ERROR: core-dn1 companion not found" >&2; exit 2;
}
[ -f "$ELEKLOADER_ROOT/mods/machines-dn1/mod.json" ] || {
  echo "ERROR: machines-dn1 companion not found" >&2; exit 2;
}

# This source set is qualified only against the exact stock OS1.44 input.
EXPECTED_STOCK_SHA256=d4f200d04484333d82822db7744e6484d0def8f2db8ddf55ee2b780cc13c9659
ACTUAL_STOCK_SHA256=$(sha256sum "$STOCK" | awk '{print $1}')
[ "$ACTUAL_STOCK_SHA256" = "$EXPECTED_STOCK_SHA256" ] || {
  echo "ERROR: stock SysEx SHA-256 mismatch" >&2
  echo "Expected: $EXPECTED_STOCK_SHA256" >&2
  echo "Actual:   $ACTUAL_STOCK_SHA256" >&2
  exit 2
}

python3 - "$ELEKLOADER_ROOT" <<'PY'
import json, pathlib, sys
root = pathlib.Path(sys.argv[1])
core = json.loads((root / "mods/core-dn1/mod.json").read_text(encoding="utf-8"))
machines = json.loads((root / "mods/machines-dn1/mod.json").read_text(encoding="utf-8"))

def ver(value):
    try:
        return tuple(int(x) for x in str(value).split(".")[:2])
    except (TypeError, ValueError):
        return ()

if ver(core.get("version")) < (5, 3):
    raise SystemExit(f"ERROR: core-dn1 {core.get('version')} is older than 5.3")
if "1.44" not in core.get("ports", {}):
    raise SystemExit("ERROR: core-dn1 has no explicit OS1.44 port")
port = machines.get("ports", {}).get("1.44")
if not port:
    raise SystemExit("ERROR: machines-dn1 has no explicit OS1.44 port")
if ver(machines.get("version")) < (0, 5):
    raise SystemExit(f"ERROR: machines-dn1 {machines.get('version')} predates the OS1.44 SDK-alpha port")
if ver(port.get("resources", {}).get("core")) < (5, 3):
    raise SystemExit("ERROR: machines-dn1 OS1.44 port does not require core 5.3+")
required = {"sdk_alpha.c", "../sdk-alpha/machine.c"}
if not required.issubset(set(port.get("sources", []))):
    raise SystemExit("ERROR: machines-dn1 OS1.44 port is missing shared SDK-alpha adapter sources")
if "sdk_alpha_machines" not in port.get("collections", {}):
    raise SystemExit("ERROR: machines-dn1 OS1.44 port does not expose sdk_alpha_machines")
for rel in ("mods/machines-dn1/sdk_alpha.c", "mods/sdk-alpha/machine.c"):
    if not (root / rel).is_file():
        raise SystemExit(f"ERROR: upstream SDK-alpha companion source missing: {rel}")
print(f"PASS upstream preflight: core-dn1 {core['version']} / machines-dn1 {machines['version']} / OS1.44")
PY

CROSS=${ELEKLOADER_CROSS:-m68k-linux-gnu-}
for tool in gcc as ld; do
  command -v "${CROSS}${tool}" >/dev/null 2>&1 || {
    echo "ERROR: missing ${CROSS}${tool}; install a compatible ColdFire toolchain" >&2
    exit 2
  }
done

OUT=${OUT_DIR:-"$ROOT/dist/dn1-lab-os1.44"}
mkdir -p "$OUT"
TMP=$(mktemp -d "${TMPDIR:-/tmp}/dn1-lab-build.XXXXXX")
trap 'rm -rf "$TMP"' EXIT HUP INT TERM
export ELEKLOADER_CROSS="$CROSS"
export PYTHONPATH="$ELEKLOADER_ROOT${PYTHONPATH:+:$PYTHONPATH}"

one_mod() {
  dir=$1
  set -- "$dir"/*.elemod
  [ -f "$1" ] || { echo "ERROR: no .elemod produced in $dir" >&2; exit 2; }
  [ "$#" -eq 1 ] || { echo "ERROR: expected one .elemod in $dir, found $#" >&2; exit 2; }
  printf '%s\n' "$1"
}
build_one() {
  source_dir=$1
  output_dir=$2
  mkdir -p "$output_dir"
  python3 -m elekloader.sdk.build "$source_dir" --stock "$STOCK" --out "$output_dir"
  one_mod "$output_dir"
}

echo "Upstream checkout: $ELEKLOADER_ROOT"
if git -C "$ELEKLOADER_ROOT" rev-parse HEAD >/dev/null 2>&1; then
  echo "Upstream commit: $(git -C "$ELEKLOADER_ROOT" rev-parse HEAD)"
else
  echo "Upstream commit: unavailable (source checkout is not a Git worktree)"
fi
echo "Stock OS1.44 SHA-256: $ACTUAL_STOCK_SHA256"

CORE=$(build_one "$ELEKLOADER_ROOT/mods/core-dn1" "$TMP/core")
COMPANION=$(build_one "$ELEKLOADER_ROOT/mods/machines-dn1" "$TMP/companion")
KARST=$(build_one "$ROOT/sdk-alpha/karst" "$TMP/karst")
MORPHANT=$(build_one "$ROOT/sdk-alpha/morphant" "$TMP/morphant")
TESSERA=$(build_one "$ROOT/sdk-alpha/tessera" "$TMP/tessera")

python3 -m elekloader.lint "$KARST" "$MORPHANT" "$TESSERA" \
  --stock "$STOCK" --with "$CORE" --with "$COMPANION"

IMAGE="$OUT/DN1-LAB-0.2.2-alpha-OS1.44.syx"
python3 -m elekloader.patch --stock "$STOCK" \
  --mod "$CORE" --mod "$COMPANION" --mod "$KARST" --mod "$MORPHANT" --mod "$TESSERA" \
  --out "$IMAGE" --version DN1LAB
sha256sum "$IMAGE" | tee "$IMAGE.sha256"
echo "Built image: $IMAGE"
echo "NOT HARDWARE CERTIFIED: validate in an emulator and on a recoverable test unit before any use."
