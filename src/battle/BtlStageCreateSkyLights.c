// bdc 0x0889dd6c BtlStageCreateSkyLights
#include "bdc.h"

/* Creates the two sky billboards of arena `arena` from its `g_btlStageSkyTable` entry and stores
   them in `g_btlSkySprites`: two "ffx_moon" sprites (blend mode 1) when `isDay` is 0, else two
   "ffx_sun" sprites (blend mode 2), the large one (size 2000) at `primaryPos` and the small one
   (size 1000) at `secondaryPos` (`BtlStageCreateSkySprite`). On arena 8 the sun positions are
   scaled by 0.9 and their heights (y) additionally by 0.6. Skipped by `BtlStageLoadMap` for
   `GameStageIs20To27Or36To39` stages. */

void BtlStageCreateSkyLights(int arena)
{
  const BtlStageSkyEntry *entry;
  float primary[4];
  float secondary[4];
  const char *name;

  entry = &g_btlStageSkyTable[arena];
  primary[0] = entry->primaryPos[0];
  primary[1] = entry->primaryPos[1];
  primary[2] = entry->primaryPos[2];
  primary[3] = 0.0f;
  secondary[0] = entry->secondaryPos[0];
  secondary[1] = entry->secondaryPos[1];
  secondary[2] = entry->secondaryPos[2];
  secondary[3] = 0.0f;

  if (entry->isDay == 0) {
    name = "ffx_moon";
    g_btlSkySprites[0] = BtlStageCreateSkySprite(2000.0f, name, primary, 1);
    g_btlSkySprites[1] = BtlStageCreateSkySprite(1000.0f, name, secondary, 1);
  } else {
    if (arena == 8) {
      /* vscl.t xyz by 0.9 (0x3f666666); the stored w is bank S713 = 0 */
      primary[0] = primary[0] * 0.899999976f;
      primary[1] = primary[1] * 0.899999976f;
      primary[2] = primary[2] * 0.899999976f;
      primary[3] = 0.0f;
      secondary[0] = secondary[0] * 0.899999976f;
      secondary[1] = secondary[1] * 0.899999976f;
      secondary[2] = secondary[2] * 0.899999976f;
      secondary[3] = 0.0f;
      primary[1] = primary[1] * 0.600000024f; /* 0x3f19999a */
      secondary[1] = secondary[1] * 0.600000024f;
    }
    name = "ffx_sun";
    g_btlSkySprites[0] = BtlStageCreateSkySprite(2000.0f, name, primary, 2);
    g_btlSkySprites[1] = BtlStageCreateSkySprite(1000.0f, name, secondary, 2);
  }
}
