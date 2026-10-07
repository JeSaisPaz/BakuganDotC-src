// bdc 0x0899d630 UiWorldMapMoveStageCursor
#include "bdc.h"

/* Moves the stage cursor `stage` of `UiWorldMap`'s stage list with up/down (pad
   repeat), wrapping within the area's stage count (`stageCount[areaGroup[areaId]]`). Returns 1 when
   it moved. */

int UiWorldMapMoveStageCursor(UiScreen *screen)

{
  UiWorldMap *map = (UiWorldMap *)screen;
  s8 old = map->stage;
  s8 next;

  if ((map->base.pad->repeat & 0x10) != 0) {
    if (old == 0) {
      next = map->stageCount[map->areaGroup[map->areaId]] - 1;
    }
    else {
      next = old - 1;
    }
    map->stage = next;
    if (map->stage != old) {
      return 1;
    }
  }
  else if ((map->base.pad->repeat & 0x40) != 0) {
    next = 0;
    if (old != map->stageCount[map->areaGroup[map->areaId]] - 1) {
      next = old + 1;
    }
    map->stage = next;
    if (map->stage != old) {
      return 1;
    }
  }
  return 0;
}
