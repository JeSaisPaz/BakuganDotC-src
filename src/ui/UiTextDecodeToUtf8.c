// bdc 0x089eac10 UiTextDecodeToUtf8
#include "bdc.h"

/* Converts a game-encoded string to UTF-8 text: decodes each glyph with `UiFontNextGlyph` and
   copies that glyph's UTF-8 sequence from `g_uiGlyphUtf8Table` (length
   `g_uiGlyphUtf8TableSize`); a glyph index past the table end writes nothing. Terminates `out`
   with 0 and returns the number of input bytes consumed. Used by `SaveBuildSfoParam` for the
   player name in the savedata title. */

s32 UiTextDecodeToUtf8(char *out, const char *text)
{
    s32 pos = 0;

    while (text[pos] != '\0') {
        s32 glyph = UiFontNextGlyph((char *)text, &pos);
        s32 index = 0;
        s32 off = 0;

        while (off < g_uiGlyphUtf8TableSize) {
            u8 lead = (u8)g_uiGlyphUtf8Table[off];
            s32 len = 1;

            /* UTF-8 lead byte: one byte per leading 1 bit. */
            if (lead & 0x80) {
                while (lead != 0) {
                    lead = (u8)(lead << 1);
                    if (!(lead & 0x80)) {
                        break;
                    }
                    len++;
                }
            }
            if (index++ == glyph) {
                s32 i;

                for (i = 0; i < len; i++) {
                    *out++ = g_uiGlyphUtf8Table[off++];
                }
                break;
            }
            off += len;
        }
    }
    *out = '\0';
    return pos;
}
