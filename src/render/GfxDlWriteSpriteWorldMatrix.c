// bdc 0x089f4cbc GfxDlWriteSpriteWorldMatrix
#include "bdc.h"

/* Writes a GE world-matrix upload to display list `dl`: command `0x3a000000` (WORLD matrix number
   0) followed by 12 data words (`0x3b` command byte) built from the 3x3 rotation/scale part of the
   4x4 matrix `m` (rows at `m+0`, `m+0x10`, `m+0x20`) and the translation vector `pos`, each float
   shifted right 8 bits (GE 24-bit floats). Returns `dl + 13`. */

u32 *GfxDlWriteSpriteWorldMatrix(u32 *dl, const float *m, const float *pos)
{
  const u32 *mw = (const u32 *)m;
  const u32 *pw = (const u32 *)pos;

  dl[0] = 0x3a000000;
  dl[1] = 0x3b000000 | (mw[0] >> 8);
  dl[2] = 0x3b000000 | (mw[1] >> 8);
  dl[3] = 0x3b000000 | (mw[2] >> 8);
  dl[4] = 0x3b000000 | (mw[4] >> 8);
  dl[5] = 0x3b000000 | (mw[5] >> 8);
  dl[6] = 0x3b000000 | (mw[6] >> 8);
  dl[7] = 0x3b000000 | (mw[8] >> 8);
  dl[8] = 0x3b000000 | (mw[9] >> 8);
  dl[9] = 0x3b000000 | (mw[10] >> 8);
  dl[10] = 0x3b000000 | (pw[0] >> 8);
  dl[11] = 0x3b000000 | (pw[1] >> 8);
  dl[12] = 0x3b000000 | (pw[2] >> 8);
  return dl + 0xd;
}
