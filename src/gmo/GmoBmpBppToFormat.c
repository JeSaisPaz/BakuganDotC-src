// bdc 0x08a27f60 GmoBmpBppToFormat
#include "bdc.h"

/* Maps a BMP bit depth to a GE pixel format: 4 → 4 (T4), 8 → 5 (T8), 16 → 1 (5551) when
   uncompressed / 0 (5650) with bitfields, 24/32 → 3 (8888); -1 otherwise. */

s32 GmoBmpBppToFormat(s32 bpp, s32 compression)
{
  s32 fmt;

  fmt = 4;
  if ((bpp != 4) && (fmt = 5, bpp != 8) && (fmt = (compression == 0), bpp != 0x10)) {
    fmt = -1;
    if (bpp == 0x18) {
      fmt = 3;
    } else if (bpp == 0x20) {
      fmt = 3;
    }
  }
  return fmt;
}
