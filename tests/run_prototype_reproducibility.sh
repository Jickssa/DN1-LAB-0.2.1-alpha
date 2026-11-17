#!/bin/sh
set -eu
ROOT=$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)
TMP=$(mktemp -d "${TMPDIR:-/tmp}/dn1-proto-repro.XXXXXX")
trap 'rm -rf "$TMP"' EXIT HUP INT TERM
for CC in gcc clang; do
  command -v "$CC" >/dev/null 2>&1 || { echo "ERROR missing compiler: $CC" >&2; exit 2; }
  mkdir -p "$TMP/$CC"
  "$CC" -std=c99 -O2 -Wall -Wextra -Werror -pedantic -I"$ROOT/host/include" \
    "$ROOT/host/prototype_demo.c" \
    "$ROOT/prototypes/vesper/machine.c" \
    "$ROOT/prototypes/mycel/machine.c" \
    "$ROOT/prototypes/orbital/machine.c" \
    "$ROOT/prototypes/lithos/machine.c" \
    "$ROOT/prototypes/photon/machine.c" \
    "$ROOT/prototypes/anima/machine.c" -o "$TMP/$CC/demo"
  "$TMP/$CC/demo" "$TMP/$CC"
done
for name in VESPER MYCEL ORBITAL LITHOS PHOTON ANIMA; do
  cmp "$TMP/gcc/$name-host-preview.wav" "$TMP/clang/$name-host-preview.wav"
  echo "PASS byte-identical WAV: $name"
done