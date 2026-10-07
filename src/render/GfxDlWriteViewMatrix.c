// bdc 0x089da820 GfxDlWriteViewMatrix
#include "bdc.h"

/* Appends 12 GE `0x2b` (VIEWMATRIXDATA) words for the 4x3 part of the row-major 4x4 float matrix
   `m` to the display list at `*dl` and advances `*dl` by 12 words. The conversion is identical to
   `GfxDlWriteWorldMatrix`; no VIEWMATRIXNUMBER command is written, so the matrix index is
   whatever the list set before. (The asm uses lwr at +1, i.e. each word is `bits >> 8`.) */

void GfxDlWriteViewMatrix(u32 **dl, const float *m)
{
  const u32 *src = (const u32 *)m;
  u32 *out = *dl;
  int row, col;

  *dl = out + 12;
  for (row = 0; row < 4; row++) {
    for (col = 0; col < 3; col++) {
      out[row * 3 + col] = src[row * 4 + col] >> 8 | 0x2b000000;
    }
  }
}
