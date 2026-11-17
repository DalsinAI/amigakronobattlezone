/* Krono's Battle Zone for GFOS: DJGPP's <dos.h>. The game uses nothing
 * from it that GFOS needs; delay() is Allegro's rest(), and rawclock()
 * (DJGPP: BIOS ticks, 18.2 a second, for the random seed and the debug
 * line) counts the same ticks from clock().
 * Copyright (c) 2026 AmiCradle Team. MIT, as Krono's Battle Zone. */
#ifndef KBZ_AMIGA_DOS_H
#define KBZ_AMIGA_DOS_H
#include <time.h>
#define delay(ms) rest(ms)
static inline long kbz_rawclock(void)
{
    return (long)((double)clock() * 18.2 / CLOCKS_PER_SEC);
}
#define rawclock() kbz_rawclock()
#endif
