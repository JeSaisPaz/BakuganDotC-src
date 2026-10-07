// bdc 0x08818330 UiFontNextGlyph
#include "bdc.h"

/* Decodes the next character of a game-encoded string at `text[*pos]` and advances `*pos`: returns
   -1 at the terminating 0; bytes `>= 0xef` map through `g_uiFontExtCodeTable``[byte - 0xef]`; bytes 200..0xee
   start a two-byte code `hi*256 + lo - 51000`; any other byte `b` is glyph `b - 1`. */

int UiFontNextGlyph(char *text, int *pos) {
    int idx = *pos;
    unsigned int c = (unsigned char)text[idx];
    if (c == 0) {
        return -1;
    }
    *pos = idx + 1;
    if (c >= 0xef) {
        /* lui/addiu 0x08a3a198 (the table), + c*4, lw -0x3bc (= -0xef*4) */
        return g_uiFontExtCodeTable[c - 0xef];
    }
    if (c >= 200) {
        int next = *pos;
        *pos = next + 1;
        c = c * 0x100 + (unsigned char)text[next] - 51000;
    }
    return c - 1;
}
