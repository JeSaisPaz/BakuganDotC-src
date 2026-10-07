// bdc 0x088cdc48 GameStageToMapId
#include "bdc.h"

/* Maps a stage number (< 0x26) to its map id through a switch (0 → 1, 1 → 2, 2 → 8, 4 → 3,
   5 → 4, 6 → 5, 8 → 13, …; 0x12 by default and for stages 3, 7, 11, 15, 19, 28-36).
   Stage 0x25 gives 0, but 0x11 when bit 0x1d of the global script flags
   (`CoreBitsetTest` on `g_scriptGlobalBits`) is set. Returns the map id. */

s32 GameStageToMapId(u32 stage)
{
  s32 map = 0x12;

  if (stage < 0x26) {
    switch (stage) {
    case 0: map = 1; break;
    case 1: map = 2; break;
    case 2: map = 8; break;
    case 4: map = 3; break;
    case 5: map = 4; break;
    case 6: map = 5; break;
    case 8: map = 0xd; break;
    case 9: map = 0xe; break;
    case 10: map = 0xf; break;
    case 0xc: map = 10; break;
    case 0xd: map = 0xb; break;
    case 0xe: map = 0xc; break;
    case 0x10: map = 6; break;
    case 0x11: map = 7; break;
    case 0x12: map = 9; break;
    case 0x14: case 0x15: case 0x16: case 0x17: map = 0x10; break;
    case 0x18: case 0x19: case 0x1a: case 0x1b: map = 0x11; break;
    case 0x25: map = 0; break;
    }
  }
  if (stage == 0x25 && CoreBitsetTest(0x1d, g_scriptGlobalBits)) {
    map = 0x11;
  }
  return map;
}
