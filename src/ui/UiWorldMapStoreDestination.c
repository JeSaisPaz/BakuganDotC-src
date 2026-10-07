// bdc 0x0899ab7c UiWorldMapStoreDestination
#include "bdc.h"

/* Stores the destination chosen on `UiWorldMap` in script global variable 1
   (`*0x08ac58c4 + 4`): outside rank mode the id for the selected area from {0x24, 3, 7, 0x13, 0xf,
   0xb, 0x25, 0x1b}; in rank mode the id for area × stage (`+0x109c * 4 + +0x109e`) from the table
   `g_worldMapStageDest`, also setting bit 0x20 of the script bitset `*0x08ac58c8` (`CoreBitsetSet`). */

void UiWorldMapStoreDestination(UiScreen *screen)
{
  UiWorldMap *map = (UiWorldMap *)screen;
  u8 ids[48];

  ids[0] = 0x24;
  ids[1] = 3;
  ids[2] = 7;
  ids[3] = 0x13;
  ids[4] = 0xf;
  ids[5] = 0xb;
  ids[6] = 0x25;
  ids[7] = 0x1b;
  memcpy(ids + 8, g_worldMapStageDest, 0x28);
  if (!UiWorldMapIsRankMode(screen)) {
    g_scriptGlobalVars[1] = ids[map->areaId];
  } else {
    CoreBitsetSet(0x20, g_scriptGlobalBits);
    g_scriptGlobalVars[1] = ids[8 + map->areaId * 4 + map->stage];
  }
}
