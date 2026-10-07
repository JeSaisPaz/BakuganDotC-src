// bdc 0x0889d614 BtlGetStageWaterHeight
#include "bdc.h"

/* Returns the water-surface height of the current arena `g_btlArenaIndex`: 41.9 for arenas 0..3,
   15.7 for 4..7, 13 for 0xc, 15 for 0xe/0xf, 20 for 0x10, 0x18 and 0x1b, 15.9 for 0x12/0x13, and 0
   for every other arena (including indices outside 0..0x1b). */
float BtlGetStageWaterHeight(void)
{
  switch (g_btlArenaIndex) {
  case 0:
  case 1:
  case 2:
  case 3:
    return 41.9000015f;
  case 4:
  case 5:
  case 6:
  case 7:
    return 15.6999998f;
  case 0xc:
    return 13.0f;
  case 0xe:
  case 0xf:
    return 15.0f;
  case 0x10:
  case 0x18:
  case 0x1b:
    return 20.0f;
  case 0x12:
  case 0x13:
    return 15.8999996f;
  default:
    return 0.0f;
  }
}
