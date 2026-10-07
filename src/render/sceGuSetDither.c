// bdc 0x08a1ebe0 sceGuSetDither
#include "bdc.h"

/* libgu sceGuSetDither: packs the 4x4 matrix (low 4 bits per entry) into
   DITH0..DITH3 (0xe2..0xe5), one row per command. */
void sceGuSetDither(const s32 *matrix)
{
  GuContext *ctx = g_guCurrentContext;
  u32 *cmd = (u32 *)ctx->listCurrent;

  cmd[0] = ((matrix[3] & 0xfU) << 12) | ((matrix[2] & 0xfU) << 8) | ((matrix[1] & 0xfU) << 4) |
           (matrix[0] & 0xfU) | 0xe2000000;
  cmd[1] = ((matrix[7] & 0xfU) << 12) | ((matrix[6] & 0xfU) << 8) | ((matrix[5] & 0xfU) << 4) |
           (matrix[4] & 0xfU) | 0xe3000000;
  cmd[2] = ((matrix[11] & 0xfU) << 12) | ((matrix[10] & 0xfU) << 8) | ((matrix[9] & 0xfU) << 4) |
           (matrix[8] & 0xfU) | 0xe4000000;
  {
    u32 row3 = ((matrix[15] & 0xfU) << 12) | ((matrix[14] & 0xfU) << 8) |
               ((matrix[13] & 0xfU) << 4) | (matrix[12] & 0xfU) | 0xe5000000;
    ctx->listCurrent = (unsigned char *)(cmd + 4);
    cmd[3] = row3;
  }
}
