/* Krono's Battle Zone for GFOS: host test of kronobz.dat through
 * gfopenallegro's allegro4 core. Every object source/code/datahead.h names
 * (the game indexes krono_dato[] with it) is in the datafile at that index
 * with that type (BMP, FONT, PAL); the bitmaps are 8-bit and the game
 * palette has 256 entries. With a third argument it writes the tank sprite
 * on the game palette as a PPM (the icon art, amiga/icons/art).
 *   test_data <kronobz.dat> <datahead.h> [tank.ppm]
 * Copyright (c) 2026 AmiCradle Team. MIT, as Krono's Battle Zone. */
#include <allegro.h>
#include <stdio.h>
#include <string.h>

static int fails;
#define CHECK(c, ...) do { if (!(c)) { fails++; printf("FAIL: " __VA_ARGS__); printf("\n"); } } while (0)

int main(int argc, char **argv)
{
    DATAFILE *df;
    FILE *h;
    char line[256], name[64], type[16];
    int idx, n = 0, count = 0, bmps = 0, fonts = 0, pals = 0, tank = -1, pal = -1;

    if (argc < 3) { printf("usage: test_data kronobz.dat datahead.h [tank.ppm]\n"); return 2; }
    CHECK(allegro_init() == 0, "allegro_init");
    set_color_depth(8);
    CHECK(set_gfx_mode(GFX_AUTODETECT, 320, 200, 0, 0) == 0, "set_gfx_mode 320x200x8");
    df = load_datafile(argv[1]);
    CHECK(df != NULL, "load_datafile %s", argv[1]);
    if (!df) return 1;
    while (df[count].type != DAT_END) count++;
    h = fopen(argv[2], "r");
    CHECK(h != NULL, "open %s", argv[2]);
    while (h && fgets(line, sizeof(line), h)) {
        if (sscanf(line, "#define %63s %d /* %15s", name, &idx, type) != 3)
            continue;
        n++;
        CHECK(idx >= 0 && idx < count, "%s index %d past the datafile's %d objects", name, idx, count);
        if (idx < 0 || idx >= count) continue;
        if (!strcmp(type, "BMP")) {
            BITMAP *b = (BITMAP *)df[idx].dat;
            CHECK(df[idx].type == DAT_BITMAP, "%s (%d) is not a bitmap", name, idx);
            if (df[idx].type == DAT_BITMAP)
                CHECK(b->w > 0 && b->h > 0 && bitmap_color_depth(b) == 8,
                      "%s: %dx%d depth %d", name, b->w, b->h, bitmap_color_depth(b));
            bmps++;
            if (!strcmp(name, "Tanque")) tank = idx;
        } else if (!strcmp(type, "FONT")) {
            CHECK(df[idx].type == DAT_FONT, "%s (%d) is not a font", name, idx);
            if (df[idx].type == DAT_FONT)
                CHECK(text_length((FONT *)df[idx].dat, "Krono") > 0, "%s draws no text", name);
            fonts++;
        } else if (!strcmp(type, "PAL")) {
            CHECK(df[idx].type == DAT_PALETTE, "%s (%d) is not a palette", name, idx);
            pals++;
            if (!strcmp(name, "Pal_Juego")) pal = idx;
        }
    }
    if (h) fclose(h);
    CHECK(n == 49 && bmps == 43 && fonts == 4 && pals == 2,
          "header: %d objects (%d BMP, %d FONT, %d PAL), not 49 (43, 4, 2)", n, bmps, fonts, pals);
    CHECK(tank >= 0 && pal >= 0, "no Tanque or Pal_Juego");
    if (argc > 3 && tank >= 0 && pal >= 0) {
        BITMAP *b = (BITMAP *)df[tank].dat;
        RGB *p = (RGB *)df[pal].dat;
        FILE *o = fopen(argv[3], "wb");
        int x, y;
        if (o) {
            fprintf(o, "P6\n%d %d\n255\n", b->w, b->h);
            for (y = 0; y < b->h; y++)
                for (x = 0; x < b->w; x++) {
                    int c = getpixel(b, x, y);
                    fputc(p[c].r * 255 / 63, o); fputc(p[c].g * 255 / 63, o); fputc(p[c].b * 255 / 63, o);
                }
            fclose(o);
        }
    }
    printf("kronobz.dat: %d objects, the header's %d (%d BMP, %d FONT, %d PAL) at their index and type\n",
           count, n, bmps, fonts, pals);
    unload_datafile(df);
    if (!fails) printf("data: all checks passed\n");
    return fails != 0;
}
