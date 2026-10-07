// bdc 0x089eb2b0 UiTextEncodeUtf8
#include "bdc.h"

/* Converts a UTF-8 string into the game's font code bytes: newline -> 0xfe; every other 1..4-byte
   sequence is looked up in `g_uiGlyphUtf8Table` (scanned up to `g_uiGlyphUtf8TableSize` bytes)
   and written as glyph index + 1, one byte below 200, else two bytes
   `((code - 200) >> 8) + 200, (code - 200) & 0xff`; a sequence not in the table writes nothing.
   Stops at NUL (writes a 0 byte) and returns the number of code bytes written, the terminator
   excluded. */

int UiTextEncodeUtf8(u8 *dst, const char *src)
{
  const s8 *s = (const s8 *)src;
  int written = 0;
  bool running = true;

  do {
    s32 c = s[0];
    u32 code = 0;
    bool end = false;

    /* The overlong-NUL checks compare a sign-extended byte (lb) against 0xc0/0xe0/0xf0/0x80, so
       they never match and only a real NUL ever ends the string (an original-code bug, kept). */
    if (c == 0) {
      end = true;
    } else if (c == 0xc0 && (s32)s[1] == 0x80) {
      end = true;
    } else if (c == 0xe0 && (s32)s[1] == 0x80 && (s32)s[2] == 0x80) {
      end = true;
    } else if (c == 0xf0 && (s32)s[1] == 0x80 && (s32)s[2] == 0x80 && (s32)s[3] == 0x80) {
      end = true;
    }

    if (end) {
      *dst++ = 0;
      running = false;
    } else if (c == '\n') {
      *dst++ = 0xfe;
      s++;
      written++;
    } else {
      s32 len;
      s32 lead = c & 0xf0;

      if ((c & 0x80) == 0) {
        len = 1;
      } else if (lead == 0xc0) {
        len = 2;
      } else if (lead == 0xe0) {
        len = 3;
      } else if (lead == 0xf0) {
        len = 4;
      } else {
        len = 0; /* stray continuation byte: src is not advanced (loops forever) */
      }

      if (len != 0) {
        s32 off = 0;
        s32 glyph = 0;

        while (off < g_uiGlyphUtf8TableSize) {
          s32 t = (s8)g_uiGlyphUtf8Table[off];
          const s8 *entry = (const s8 *)&g_uiGlyphUtf8Table[off];
          bool match;

          if (len == 1) {
            match = c == t;
          } else if (len == 2) {
            match = c == t && s[1] == entry[1];
          } else if (len == 3) {
            match = (t & 0xf0) == 0xe0 && c == t && s[1] == entry[1] && s[2] == entry[2];
          } else {
            match = (t & 0xf0) == 0xf0 && c == t && s[1] == entry[1] && s[2] == entry[2] &&
                    s[3] == entry[3];
          }
          if (match) {
            code = glyph + 1;
            break;
          }
          /* Skip the entry: one byte per leading 1 bit of its lead byte. */
          if ((t & 0x80) == 0) {
            off++;
          } else {
            u32 bits = (u32)t;

            do {
              bits <<= 1;
              off++;
            } while (bits & 0x80);
          }
          glyph++;
        }
        s += len;
      }

      if (code != 0) {
        if (code < 200) {
          *dst++ = (u8)code;
          written++;
        } else {
          code -= 200;
          *dst++ = (u8)((code >> 8) + 200);
          *dst++ = (u8)code;
          written += 2;
        }
      }
    }
  } while (running);

  return written;
}
