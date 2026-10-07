// bdc 0x0899d5d4 UiWorldMapCheckRandomButton
#include "bdc.h"

/* Checks the triangle button (pad pressed `0x1000`, random pick) on `UiWorldMap`'s
   area list when `randomEnabled` is set: returns 0 if not pressed, 1 if at least one area is known
   (`unlockMask` non-zero), 2 otherwise. A 1 starts the random area pick of main-phase steps 0xc/0xd
   (`UiWorldMapAreaRoulette`). */

int UiWorldMapCheckRandomButton(UiScreen *screen)
{
  UiWorldMap *map = (UiWorldMap *)screen;
  s32 i;

  if (map->randomEnabled != 0 && (screen->pad->pressed & 0x1000) != 0) {
    i = 0;
    do {
      if ((map->unlockMask & (1 << i)) != 0) {
        return 1;
      }
      i++;
    } while (i < 8);
    return 2;
  }
  return 0;
}
