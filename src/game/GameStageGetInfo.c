// bdc 0x089b1e80 GameStageGetInfo
#include "bdc.h"

/* Copies the 0x14-byte (10 x u16) info record of a story stage into `out`: record `areaBase[area] +
   slot` of `g_gameStageInfoTable`, with `areaBase` the byte table `g_gameStageRecAreaBase`. Callers pass
   `area = stage >> 2` and `slot = stage & 3` of the story stage (script global 1, `g_scriptGlobalVars +
   4`). */

void GameStageGetInfo(u16 *out, u8 area, u8 slot)
{
  const u16 *rec;
  s32 i;

  rec = &g_gameStageInfoTable[(g_gameStageRecAreaBase[area] + slot) * 10];
  for (i = 0; i < 10; i++) {
    out[i] = rec[i];
  }
}
