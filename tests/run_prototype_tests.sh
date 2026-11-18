#!/bin/sh
set -eu
ROOT=$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)
CC=${CC:-cc}
OUT="$ROOT/host/prototype_test"
mkdir -p "$ROOT/audio-prototypes"
python3 "$ROOT/tests/validate_prototypes.py"
"$CC" -std=c99 -O2 -DDN1_LAB_METER_CLIPS -Wall -Wextra -Werror -pedantic \
  -I"$ROOT/host/include" \
  "$ROOT/host/prototype_test.c" \
  "$ROOT/prototypes/vesper/machine.c" \
  "$ROOT/prototypes/mycel/machine.c" \
  "$ROOT/prototypes/orbital/machine.c" \
  "$ROOT/prototypes/lithos/machine.c" \
  "$ROOT/prototypes/photon/machine.c" \
  "$ROOT/prototypes/anima/machine.c" \
  -o "$OUT"
"$OUT"