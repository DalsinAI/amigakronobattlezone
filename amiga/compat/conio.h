/* Krono's Battle Zone for GFOS: DJGPP's <conio.h>, for the text-mode
 * messages the game prints before and after its Allegro screen. On GFOS
 * they go to the Shell (stdout); colours and cursor moves are dropped.
 * Copyright (c) 2026 AmiCradle Team. MIT, as Krono's Battle Zone. */
#ifndef KBZ_AMIGA_CONIO_H
#define KBZ_AMIGA_CONIO_H
#include <stdio.h>

enum { BLACK, BLUE, GREEN, CYAN, RED, MAGENTA, BROWN, LIGHTGRAY, DARKGRAY,
       LIGHTBLUE, LIGHTGREEN, LIGHTCYAN, LIGHTRED, LIGHTMAGENTA, YELLOW, WHITE };

#define textcolor(c)       ((void)(c))
#define textbackground(c)  ((void)(c))
#define clrscr()           ((void)0)
#define gotoxy(x, y)       ((void)(x), (void)(y))
#define window(l, t, r, b) ((void)(l), (void)(t), (void)(r), (void)(b))
#define cprintf            printf
#define cputs(s)           fputs((s), stdout)
/* the Shell has no raw key wait: carry on (each call is a "press a key") */
#define getch()            (fflush(stdout), 0)
#define kbhit()            0
#endif
