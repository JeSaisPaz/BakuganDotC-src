// bdc 0x089ea8e4 UiFontEncodeChar
#include "bdc.h"

/* Encodes one character (ASCII, half-width katakana 0xa0..0xdf, or a two-byte SJIS pair) into the
   game's font code: looks it up in the glyph table `g_uiFontGlyphTable` (length
   `g_uiFontGlyphTableLen`) and writes the 1-based index as one byte (< 200) or as two bytes
   (`0xc8 + hi`, `lo` of `index - 200`). Returns the number of bytes written, 0 when unmapped. */

enum { CHAR_ASCII, CHAR_KANA, CHAR_SJIS };

s32 UiFontEncodeChar(u8 *out, const u8 *ch)
{
    u8 c = ch[0];
    int kind = CHAR_SJIS;
    u32 code = 0;
    int glyph = 0;
    int pos;

    if ((s8)c >= 0) {
        kind = CHAR_ASCII;
    }
    if (c >= 0xa0 && c < 0xe0) {
        kind = CHAR_KANA;
    }
    for (pos = 0; pos < g_uiFontGlyphTableLen; glyph++) {
        u8 t = (u8)g_uiFontGlyphTable[pos];

        if ((s8)t >= 0) {
            if (kind == CHAR_ASCII && t == c) {
                code = glyph + 1;
                break;
            }
            pos += 1;
        } else if (t >= 0xa0 && t < 0xe0) {
            if (kind == CHAR_KANA && t == c) {
                code = glyph + 1;
                break;
            }
            pos += 1;
        } else {
            if (kind == CHAR_SJIS && t == c && (u8)g_uiFontGlyphTable[pos + 1] == ch[1]) {
                code = glyph + 1;
                break;
            }
            pos += 2;
        }
    }
    if (code == 0) {
        return 0;
    }
    if (code < 200) {
        out[0] = (u8)code;
        return 1;
    }
    out[0] = (u8)(((code - 200) >> 8) + 200);
    out[1] = (u8)(code - 200);
    return 2;
}
