#!/bin/sh
# Krono's Battle Zone for GFOS: host tests.
#   tests/host/run.sh [GFOPENALLEGRO/allegro4]
# Through gfopenallegro's allegro4 core on its fake acgame.library:
#   - test_data: every object source/code/datahead.h names is in
#     kronobz.dat at its index with its type, 8-bit;
#   - the whole game (principa.c built as on GFOS, -D__amigaos__ with
#     amiga/compat) links on the host, so no Allegro call is missing, and a
#     smoke run with kronobz.dat shows its menu (smoke_probe.c).
# Copyright (c) 2026 AmiCradle Team. MIT, as Krono's Battle Zone.
set -eu
ROOT=$(CDPATH= cd -- "$(dirname -- "$0")/../.." && pwd)
A4=${1:-"$ROOT/../gfopenallegro/allegro4"}
CC=${HOSTCC:-cc}
T=$(mktemp -d)
trap 'rm -rf "$T"' EXIT HUP INT TERM
mkdir -p "$T/a4" "$T/run/User"
for f in "$A4"/src/a4_*.c "$A4"/tests/fake_acgame.c; do
    "$CC" -std=gnu99 -D_DEFAULT_SOURCE -w -I"$A4/include" -I"$A4/src" -I"$A4/tests" -c "$f" -o "$T/a4/$(basename "$f").o"
done
DAT="$ROOT/release/msdos_or_dosbox/kronobz.dat"
"$CC" -std=gnu99 -w -I"$A4/include" "$ROOT/tests/host/test_data.c" "$T"/a4/*.o -lm -o "$T/test_data"
"$T/test_data" "$DAT" "$ROOT/source/code/datahead.h"
[ "${HOST_LINK:-1}" = 0 ] && { echo "game: host link skipped (HOST_LINK=0)"; exit 0; }
"$CC" -std=gnu17 -w -D__amigaos__ -I"$ROOT/amiga/compat" -I"$A4/include" -c "$ROOT/source/code/principa.c" -o "$T/game.o"
"$CC" -std=gnu99 -w -I"$A4/include" -I"$A4/tests" -c "$ROOT/tests/host/smoke_probe.c" -o "$T/probe.o"
"$CC" -o "$T/kbz" "$T/game.o" "$T/probe.o" "$T"/a4/*.o -lm -lpthread
echo "game: links on the host"
cp "$DAT" "$T/run/"
(cd "$T/run" && AT_SMOKE_SECS=${AT_SMOKE_SECS:-8} AT_SMOKE_PPM=${AT_SMOKE_PPM:-} "$T/kbz" | tr '\r' '\n' | grep '^smoke:')
