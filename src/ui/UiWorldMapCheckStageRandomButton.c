// bdc 0x0899eca4 UiWorldMapCheckStageRandomButton
#include "bdc.h"

/* Checks the triangle button (pad pressed `0x1000`) in `UiWorldMap`'s stage list
   when `+0x10bb` is set: returns 0 if not pressed, 1 if the area has a cleared stage
   (`+0x11c2[areaId]`), 2 otherwise. With 1 and select also held the main phase starts
   `UiWorldMapStageRoulette`. */

int UiWorldMapCheckStageRandomButton(UiScreen *screen)
{
  UiWorldMap *map = (UiWorldMap *)screen;

  if (map->randomEnabled != 0 && (screen->pad->pressed & 0x1000) != 0) {
    if (map->clearedStage[map->areaGroup[map->areaId]] != 0) {
      return 1;
    }
    return 2;
  }
  return 0;
}
