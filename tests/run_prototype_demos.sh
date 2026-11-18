#!/bin/sh
set -eu
ROOT=$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)
CC=${CC:-cc}
OUT="$ROOT/host/prototype_demo"
mkdir -p "$ROOT/audio-prototypes"
"$CC" -std=c99 -O2 -Wall -Wextra -Werror -pedantic -I"$ROOT/host/include" \
  "$ROOT/host/prototype_demo.c" \
  "$ROOT/prototypes/vesper/machine.c" "$ROOT/prototypes/mycel/machine.c" \
  "$ROOT/prototypes/orbital/machine.c" "$ROOT/prototypes/lithos/machine.c" \
  "$ROOT/prototypes/photon/machine.c" "$ROOT/prototypes/anima/machine.c" -o "$OUT"
"$OUT" "$ROOT/audio-prototypes"
