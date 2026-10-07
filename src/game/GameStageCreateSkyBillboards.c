// bdc 0x088d42c4 GameStageCreateSkyBillboards
#include "bdc.h"

/* Creates two sky billboards for `stage` from `g_gameStageSunTable` (7 words per stage: two
   positions x, y, z and a day flag read as an int): `"ffx_moon"` (sizes 2000 and 1000, id 1) when
   the flag is 0, otherwise `"ffx_sun"` (id 2) with both positions scaled by 0.7 and their heights by
   a further 0.6 (`GameStageCreateBillboard`). The w lanes are 0 (on the sun path the asm's
   `sv.q` overwrites them with a stale VFPU lane, which the C
   does not reproduce). */

void GameStageCreateSkyBillboards(s32 stage)
{
  float p[8];
  const float *e;

  e = g_gameStageSunTable[stage];
  p[0] = e[0];
  p[1] = e[1];
  p[2] = e[2];
  p[3] = 0.0f;
  p[4] = e[3];
  p[5] = e[4];
  p[6] = e[5];
  p[7] = 0.0f;
  if (*(const s32 *)&e[6] == 0) {
    GameStageCreateBillboard(2000.0f, "ffx_moon", &p[0], 1);
    GameStageCreateBillboard(1000.0f, "ffx_moon", &p[4], 1);
  } else {
    p[0] = p[0] * 0.7f;
    p[1] = p[1] * 0.7f;
    p[2] = p[2] * 0.7f;
    p[4] = p[4] * 0.7f;
    p[5] = p[5] * 0.7f;
    p[6] = p[6] * 0.7f;
    p[1] = p[1] * 0.6f;
    p[5] = p[5] * 0.6f;
    GameStageCreateBillboard(2000.0f, "ffx_sun", &p[0], 2);
    GameStageCreateBillboard(1000.0f, "ffx_sun", &p[4], 2);
  }
}
