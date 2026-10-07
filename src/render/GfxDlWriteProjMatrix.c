// bdc 0x089e2818 GfxDlWriteProjMatrix
#include "bdc.h"

/* Writes a GE projection matrix: command `PROJ_START` (`0x3e000000`) followed by the 16 floats of
   `m` as data words (`0x3f` command byte, float bits >> 8). The asm reads `m->y.w` with a
   byte-offset-3 `lwr`, so that one word carries only the top byte of the float in its low byte
   (`0x3f0000xx`-style merge with the previous word); reproduced here. Returns `list + 17`. */

u32 *GfxDlWriteProjMatrix(u32 *list, const ScePspFMatrix4 *m)
{
  const u32 *w = (const u32 *)m;
  u32 prev;

  list[0] = 0x3e000000;
  list[1] = 0x3f000000 | (w[0] >> 8);
  list[2] = 0x3f000000 | (w[1] >> 8);
  list[3] = 0x3f000000 | (w[2] >> 8);
  list[4] = 0x3f000000 | (w[3] >> 8);
  list[5] = 0x3f000000 | (w[4] >> 8);
  list[6] = 0x3f000000 | (w[5] >> 8);
  list[7] = 0x3f000000 | (w[6] >> 8);
  prev = list[4];
  list[8] = (prev & 0xffffff00) | (w[7] >> 24);
  list[9] = 0x3f000000 | (w[8] >> 8);
  list[10] = 0x3f000000 | (w[9] >> 8);
  list[11] = 0x3f000000 | (w[10] >> 8);
  list[12] = 0x3f000000 | (w[11] >> 8);
  list[13] = 0x3f000000 | (w[12] >> 8);
  list[14] = 0x3f000000 | (w[13] >> 8);
  list[15] = 0x3f000000 | (w[14] >> 8);
  list[16] = 0x3f000000 | (w[15] >> 8);
  return list + 0x11;
}
