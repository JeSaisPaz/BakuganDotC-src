// bdc 0x089da794 GfxDlWriteWorldMatrix
#include "bdc.h"

/* Appends a world-matrix upload to the display list at `*dl` and advances `*dl` by 13 words: GE
   command `0x3a` (WORLDMATRIXNUMBER, index 0) followed by 12 `0x3b` (WORLDMATRIXDATA) words. The
   data words are the 4x3 part of the row-major 4x4 float matrix `m` (three floats of each of the
   four 16-byte rows; the fourth float of every row is skipped), each shifted right by 8 bits into
   the 24-bit GE float format and OR-ed with the command byte. */

void GfxDlWriteWorldMatrix(u32 **dl, const float *m)
{
  const u32 *src = (const u32 *)m;
  u32 *out = *dl;
  int row, col;

  *dl = out + 13;
  *out = 0x3a000000;
  for (row = 0; row < 4; row++) {
    for (col = 0; col < 3; col++) {
      out[1 + row * 3 + col] = src[row * 4 + col] >> 8 | 0x3b000000;
    }
  }
}
