# Krono's Battle Zone for GFOS

Krono's Battle Zone 1.00 by Alvaro "Krono" Gonzalez Ferrer (Kronoman,
MIT), from the author's own
https://github.com/alvarogonzalezferrer/krono_battle_zone at commit
33f71949a15e231b8ee1c811972540aa1bc3dbf3, built for AmigaOS 3.2 / GFOS
on a 68040, 68060 or 68080. No FPU is needed.

A tank battle for two players on a split screen, or one against the
computer, written in 2001 for MS-DOS: drive around a large random map,
collect power-ups (shields, weapons, radar, repairs) and blow the other
tank up, in rain, snow, fog, sand storms, day or night, on ground that
breaks under fire. It runs in 320 x 200 (or 320 x 240 for the menus) in
256 colours. The menus are in Spanish.

Status: see the pull request (stove build and lab).

## What is in the drawer

- `KronoBattleZone`: the game.
- `kronobz.dat`: the author's datafile (sprites, fonts, palettes), as
  released.
- `Libs/allegro4.library` and `Libs/acgame.library`: Allegro 4 for GFOS
  (gfopenallegro) on ACGame. The game opens them from its own `Libs/`.
- `User/`: your settings (`krbz.cfg`), made by the game. Updates and
  `GFOpenUpTool REMOVE` leave it alone.
- `Controls`, `ReadMe-KronoBattleZone` (the author's read-me),
  `Licences.txt`, `LICENSE.txt`.

## Playing

Run Install-KronoBattleZone (or copy the KronoBattleZone drawer where
you want it), then double-click KronoBattleZone, or start it from a
Shell from anywhere: the game makes its own drawer its current directory
while it runs. The author's command line options work (`-nosplit`,
`-noluz`, `-r640480` and so on, see the read-me). The keys are in
`Controls`.

## How the port is made

- The code is the author's, built against gfopenallegro's Allegro 4 with
  68040 soft float (`amiga/Makefile.amigaos`). It is one unit,
  `principa.c`, as the author's `make.bat` builds it.
- DJGPP's `<conio.h>` and `<dos.h>` are stood in for by `amiga/compat`:
  the text the game prints before and after its screen goes to the
  Shell, `delay` is Allegro's `rest`, and `rawclock` (BIOS ticks) counts
  18.2 ticks a second from `clock()`.
- `kronobz.dat` is read from the drawer, and `krbz.cfg` goes in `User/`,
  instead of beside `argv[0]` (`source/code/principa.c`, marked
  `__amigaos__`).
- The game never had sound; nothing is missing there.

## Building

    make -f amiga/Makefile.amigaos PREFIX=<stove>/prefix A4=<gfopenallegro>/allegro4 A4OUT=... ACGOUT=...

or, in Cradle, stove_build with the os32-gcc16-open6 stove and target
package (`package/build-amiga.sh`), which also builds allegro4.library
and acgame.library at the commits `package/package.env` pins, and packs
KronoBattleZone-VERSION-68040.lha and the GFOpenUp part.

    tests/host/run.sh <gfopenallegro>/allegro4

runs the host tests: every object `source/code/datahead.h` names (43
bitmaps, 4 fonts, 2 palettes) is in `kronobz.dat` at that index with
that type, 8-bit; the whole game links on the host against gfopenallegro;
and a smoke run with `kronobz.dat` shows its menu.

Designed, tested and delivered by The AmiCradle Team.
