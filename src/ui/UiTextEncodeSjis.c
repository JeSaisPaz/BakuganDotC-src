// bdc 0x089ead50 UiTextEncodeSjis
#include "bdc.h"

/* Converts a NUL-terminated Shift-JIS/ASCII string into the game's font code bytes with
   `UiFontEncodeChar` (one- or two-byte glyph codes; SJIS pairs consume two source bytes).
   Characters the font lacks become 0xfd, newline 0xfe, and some lowercase letters map to special
   codes ('b' 0xf6, 'c' 0xf0, 'd' 0xfb, 'f' 0xf2, 'g' 0xf7, 'h' 0xef, 'k' 0xf9, 'l' 0xf3, 'n' 0xf1,
   'r' 0xf8, 's' 0xf4, 'w' 0xfa). Returns the number of glyphs found in the font. */

int UiTextEncodeSjis(u8 *dst, const char *src)
{
  u8 c;
  s32 len;
  int pos;
  int found;

  c = src[0];
  pos = 0;
  found = 0;
  while (c != 0) {
    len = UiFontEncodeChar(dst, (const u8 *)(src + pos));
    if (len != 0) {
      pos += len;
      dst += len;
      found++;
      c = src[pos];
      continue;
    }
    if (c < 'b') {
      *dst = (c == '\n') ? 0xfe : 0xfd;
    } else {
      switch (c) {
      case 'b': *dst = 0xf6; break;
      case 'c': *dst = 0xf0; break;
      case 'd': *dst = 0xfb; break;
      case 'f': *dst = 0xf2; break;
      case 'g': *dst = 0xf7; break;
      case 'h': *dst = 0xef; break;
      case 'k': *dst = 0xf9; break;
      case 'l': *dst = 0xf3; break;
      case 'n': *dst = 0xf1; break;
      case 'r': *dst = 0xf8; break;
      case 's': *dst = 0xf4; break;
      case 'w': *dst = 0xfa; break;
      default: *dst = 0xfd; break;
      }
    }
    dst++;
    pos++;
    c = src[pos];
  }
  *dst = 0;
  return found;
}
