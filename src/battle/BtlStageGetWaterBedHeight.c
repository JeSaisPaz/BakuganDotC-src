// bdc 0x0889d6a4 BtlStageGetWaterBedHeight
#include "bdc.h"

/* Returns a per-arena negative offset plus the current arena's water surface
   (`BtlGetStageWaterHeight`): -90.9 for arenas 0..3, -35.5 for 4..7, -38 for 0xc, -40 for
   0xe..0x10, -25.9 for 0x12/0x13, -60 for 0x18/0x1b, 0 for every other arena. */
float BtlStageGetWaterBedHeight(void)
{
  float offset = 0.0f;

  switch (g_btlArenaIndex) {
  case 0:
  case 1:
  case 2:
  case 3:
    offset = -90.9000015f;
    break;
  case 4:
  case 5:
  case 6:
  case 7:
    offset = -35.5f;
    break;
  case 0xc:
    offset = -38.0f;
    break;
  case 0xe:
  case 0xf:
  case 0x10:
    offset = -40.0f;
    break;
  case 0x12:
  case 0x13:
    offset = -25.8999996f;
    break;
  case 0x18:
  case 0x1b:
    offset = -60.0f;
    break;
  default:
    break;
  }
  return offset + BtlGetStageWaterHeight();
}
