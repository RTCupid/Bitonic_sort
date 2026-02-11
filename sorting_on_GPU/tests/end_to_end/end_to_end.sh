#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(CDPATH= cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
DATA_DIR="$SCRIPT_DIR/data"
ANSW_DIR="$SCRIPT_DIR/answ"

BIN="${1:-$SCRIPT_DIR/../../../build/sorting_on_GPU/sort_gpu}"


if [[ ! -x "$BIN" ]]; then
  echo "ERROR: binary not found or not executable: $BIN" >&2
  exit 1
fi
if [[ ! -d "$DATA_DIR" ]]; then
  echo "ERROR: data dir not found: $DATA_DIR" >&2
  exit 1
fi
if [[ ! -d "$ANSW_DIR" ]]; then
  echo "ERROR: answ dir not found: $ANSW_DIR" >&2
  exit 1
fi

normalize() {
  tr -s '[:space:]' ' ' | sed 's/^ *//; s/ *$//'
}

tmp_out="$(mktemp)"
tmp_exp="$(mktemp)"
trap 'rm -f "$tmp_out" "$tmp_exp"' EXIT

found=0
for in_file in "$DATA_DIR"/*.txt; do
  [[ -e "$in_file" ]] || continue
  found=1

  name="$(basename "$in_file")"
  exp_file="$ANSW_DIR/$name"

  if [[ ! -f "$exp_file" ]]; then
    echo "ERROR: expected answer file not found: $exp_file" >&2
    exit 1
  fi

  "$BIN" < "$in_file" | normalize > "$tmp_out"
  normalize < "$exp_file" > "$tmp_exp"

  if diff -u "$tmp_exp" "$tmp_out" >/dev/null; then
    echo "OK: $name"
  else
    echo "FAIL: $name" >&2
    diff -u "$tmp_exp" "$tmp_out" || true
    exit 1
  fi
done

if [[ "$found" -eq 0 ]]; then
  echo "ERROR: no input files found in $DATA_DIR (*.txt)" >&2
  exit 1
fi

echo "all tests passed"
