// bdc 0x08998288 UiWorldMapGetGlobeMotion
#include "bdc.h"

/* Picks the globe animation for the selected slot `areaId` of `UiWorldMap` from
   its bit in the unlocked-area mask `unlockMask` (`UiWorldMapInitState`): 0 = unlocked area other
   than slot 0 (turn the globe to it), 1 = locked area (idle spin), 2 = slot 0 unlocked (spin with
   Marucho's jet rings). */

int UiWorldMapGetGlobeMotion(UiScreen *screen)
{
  UiWorldMap *map = (UiWorldMap *)screen;
  s32 slot = map->areaId;
  u32 bit = map->unlockMask & (1 << (slot & 0x1f));

  if (slot == 0) {
    return bit != 0 ? 2 : 1;
  }
  if (bit == 0) {
    return 1;
  }
  return 0;
}
