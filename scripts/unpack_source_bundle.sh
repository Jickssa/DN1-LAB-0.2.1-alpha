#!/bin/sh
set -eu

ROOT=$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)
BUNDLE="$ROOT/source-bundles/dn1-lab-0.2.2-alpha-host-source.tar.gz"
CHECKSUMS="$ROOT/source-bundles/SHA256SUMS.txt"

command -v sha256sum >/dev/null 2>&1 || {
  echo "sha256sum is required" >&2
  exit 1
}
command -v tar >/dev/null 2>&1 || {
  echo "tar is required" >&2
  exit 1
}
[ -f "$BUNDLE" ] || {
  echo "Missing source bundle: $BUNDLE" >&2
  exit 1
}

(
  cd "$ROOT/source-bundles"
  sha256sum -c "$(basename "$CHECKSUMS")"
)

TMP=$(mktemp -d)
trap 'rm -rf "$TMP"' EXIT HUP INT TERM

tar -tzf "$BUNDLE" > "$TMP/paths.txt"
while IFS= read -r entry; do
  case "$entry" in
    /*|../*|*/../*|*/..)
      echo "Unsafe archive path rejected: $entry" >&2
      exit 1
      ;;
  esac
done < "$TMP/paths.txt"

tar -xzf "$BUNDLE" -C "$ROOT"
echo "DN1-LAB host sources extracted and archive digest verified."
