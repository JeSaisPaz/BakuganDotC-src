// bdc 0x089970b8 UiWorldMapScrollLines
#include "bdc.h"

/* Advances the UV scroll `+0x21b0` of the `"psp_line__BA"` material of the
   `UiWorldMap` model by 1/1200 per frame, wrapped to [0, 1) (hooked by
   `UiWorldMapHookLineMaterial`). */

void UiWorldMapScrollLines(UiScreen *screen)
{
  UiWorldMap *map = (UiWorldMap *)screen;
  float v;

  v = map->lineUv[0] + 0.00083333335f;
  map->lineUv[0] = v;
  if (v < 0.0f) {
    v = v + 1.0f;
    map->lineUv[0] = v;
  }
  if (!(v < 1.0f)) {
    map->lineUv[0] = v - 1.0f;
  }
}
