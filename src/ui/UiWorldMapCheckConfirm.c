// bdc 0x0899ba80 UiWorldMapCheckConfirm
#include "bdc.h"

/* Checks the confirm button (pad pressed `0x4000`) on `UiWorldMap`: returns 0 if
   not pressed, 1 if the selected area is enterable (bit set in `+0x10b9` and clear in the blocked
   mask `+0x10ba`), 2 otherwise (error sound in the main phase). */

int UiWorldMapCheckConfirm(UiScreen *screen)
{
  UiWorldMap *map = (UiWorldMap *)screen;
  u32 bit;

  if ((screen->pad->pressed & 0x4000) != 0) {
    bit = 1 << (map->areaId & 0x1f);
    if ((map->unlockMask & bit) == 0 || (map->blockedMask & bit) != 0) {
      return 2;
    }
    return 1;
  }
  return 0;
}
