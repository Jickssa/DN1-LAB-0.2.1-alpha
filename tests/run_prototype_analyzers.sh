#!/bin/sh
set -eu
ROOT=$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)
for name in vesper mycel orbital lithos photon anima; do
  source="$ROOT/prototypes/$name/machine.c"
  gcc -std=c99 -Wall -Wextra -Werror -pedantic -fanalyzer \
    -I"$ROOT/host/include" -fsyntax-only "$source"
  clang --analyze -std=c99 -I"$ROOT/host/include" "$source" \
    -o "${TMPDIR:-/tmp}/dn1-${name}.plist"
  echo "PASS static analysis: $name"
done