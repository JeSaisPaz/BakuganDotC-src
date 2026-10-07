// bdc 0x0899d6ec UiWorldMapCheckStageConfirm
#include "bdc.h"

/* Checks the confirm button (pad pressed `0x4000`) in `UiWorldMap`'s stage list:
   returns 0 if not pressed, 1 if the selected stage `+0x109e` is cleared (below `clearedStage[areaGroup[areaId]]`, `+0x11c2[+0x10a0[areaId]]`),
   2 otherwise. */

int UiWorldMapCheckStageConfirm(UiScreen *screen)
{
  UiWorldMap *map = (UiWorldMap *)screen;

  if ((screen->pad->pressed & 0x4000) != 0) {
    if (map->stage < map->clearedStage[map->areaGroup[map->areaId]]) {
      return 1;
    }
    return 2;
  }
  return 0;
}
