#!/bin/sh
set -eu
ROOT=$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)
for CC in gcc clang; do
  command -v "$CC" >/dev/null 2>&1 || { echo "SKIP $CC unavailable"; continue; }
  OUT="${TMPDIR:-/tmp}/dn1-prototype-${CC}-san"
  "$CC" -std=c99 -O1 -g -fsanitize=address,undefined -fno-omit-frame-pointer \
    -DDN1_LAB_METER_CLIPS -Wall -Wextra -Werror -pedantic \
    -I"$ROOT/host/include" \
    "$ROOT/host/prototype_test.c" \
    "$ROOT/prototypes/vesper/machine.c" \
    "$ROOT/prototypes/mycel/machine.c" \
    "$ROOT/prototypes/orbital/machine.c" \
    "$ROOT/prototypes/lithos/machine.c" \
    "$ROOT/prototypes/photon/machine.c" \
    "$ROOT/prototypes/anima/machine.c" -o "$OUT"
  ASAN_OPTIONS=detect_leaks=1 UBSAN_OPTIONS=halt_on_error=1 "$OUT" >"$OUT.log"
  tail -1 "$OUT.log"
done