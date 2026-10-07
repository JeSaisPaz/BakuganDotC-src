// bdc 0x08997110 UiWorldMapScrollSea
#include "bdc.h"

/* Moves the UV scroll `+0x21c0` of the `"psp_sea__DS2"` material of the
   `UiWorldMap` model back by 1/1200 per frame, wrapped to [0, 1) (hooked by
   `UiWorldMapHookSeaMaterial`). */

void UiWorldMapScrollSea(UiScreen *screen)
{
  UiWorldMap *map = (UiWorldMap *)screen;
  float v;

  v = map->seaUv[0] - 0.00083333335f;
  map->seaUv[0] = v;
  if (v < 0.0f) {
    v = v + 1.0f;
    map->seaUv[0] = v;
  }
  if (!(v < 1.0f)) {
    map->seaUv[0] = v - 1.0f;
  }
}
