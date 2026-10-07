// bdc 0x08817be8 UiFontNextGlyphSjis
#include "bdc.h"

/* Shift-JIS variant of `UiFontNextGlyph` used by `UiTextPrinterPrintSjis`: reads one character
   at `text[*pos]` and returns its glyph index, writing the glyph width (6.0 for half-width, 12.0
   for full-width) to `*outWidth`; returns -1 at the end (also for a lead byte followed by NUL),
   -3 for `\n`, -4 for byte 1 and -2 for a double-byte code whose trail byte is below 0x20. The
   kanji listed in `g_uiFontSjisRemapTable` map to 0x5e8 + their index. */

int UiFontNextGlyphSjis(char *text, int *pos, float *outWidth)
{
    int lead;
    int trail;
    int row;
    int cell;
    unsigned int i;

    lead = (unsigned char)text[*pos];
    if (lead == 0) {
        return -1;
    }
    *pos = *pos + 1;
    if (lead == '\n') {
        return -3;
    }
    if (lead == 1) {
        return -4;
    }
    trail = (unsigned char)text[*pos];
    *outWidth = 12.0f;
    if (lead >= 0xa1 && lead < 0xe0) {
        /* half-width katakana */
        *outWidth = 6.0f;
        return lead - 0x41;
    }
    if (lead < 0x81) {
        /* ASCII */
        *outWidth = 6.0f;
        return lead - 0x20;
    }
    if (trail == 0) {
        return -1;
    }
    *pos = *pos + 1;
    if (trail < 0x20) {
        return -2;
    }
    if (lead >= 0x88 && lead < 0x98 && (trail == 0xfc || trail == 0x9e)) {
        for (i = 0; i < 0x3e; i += 2) {
            if (g_uiFontSjisRemapTable[i] == lead && g_uiFontSjisRemapTable[i + 1] == trail) {
                return ((int)i >> 1) + 0x5e8;
            }
        }
    }
    if (lead >= 0xe0) {
        lead -= 0x40;
    }
    if (trail >= 0x80) {
        trail -= 1;
    }
    if (trail < 0x9e) {
        row = lead * 2 - 0x102;
        cell = trail - 0x40;
    } else {
        row = lead * 2 - 0x101;
        cell = trail - 0x9e;
    }
    return row * 0x5d + cell + 0xbd;
}
