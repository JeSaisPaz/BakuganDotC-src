// bdc 0x0899b9ac UiWorldMapMoveCursor
#include "bdc.h"

/* Moves the area cursor `+0x109c` of `UiWorldMap` with up/down (pad repeat
   0x10/0x40) to the previous/next selectable area (bit set in `+0x10b8`), wrapping around the 8
   areas. Returns 1 when moved. */

int UiWorldMapMoveCursor(UiScreen *screen)
{
  UiWorldMap *map = (UiWorldMap *)screen;
  s8 area;
  int tries;

  if ((screen->pad->repeat & 0x10) != 0) {
    tries = 0;
    area = map->areaId;
    do {
      area = (area == 0) ? 7 : (s8)(area - 1);
      tries++;
    } while ((map->selectableMask & (1 << (area & 0x1f))) == 0 && tries < 8);
    map->areaId = area;
    return 1;
  }
  if ((screen->pad->repeat & 0x40) != 0) {
    tries = 0;
    area = map->areaId;
    do {
      area = (area == 7) ? 0 : (s8)(area + 1);
      tries++;
    } while ((map->selectableMask & (1 << (area & 0x1f))) == 0 && tries < 8);
    map->areaId = area;
    return 1;
  }
  return 0;
}
