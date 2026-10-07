// bdc 0x08998000 UiWorldMapHookLineMaterial
#include "bdc.h"

/* Resets the UV scroll vector `lineUv` of `UiWorldMap` and registers it with the
   `"psp_line__BA"` material of the map model `mapModel` (`GfxModelSetMaterialAnimCallback` with callback `0x089963b0`);
   animated by `UiWorldMapScrollLines`. */

void UiWorldMapHookLineMaterial(UiScreen *screen)
{
  UiWorldMap *map = (UiWorldMap *)screen;
  GfxModel *self = map->mapModel;

  map->lineUv[0] = 0.0f;
  map->lineUv[1] = 0.0f;
  map->lineUv[2] = 0.0f;
  map->lineUv[3] = 0.0f;
  GfxModelSetMaterialAnimCallback(self, "psp_line__BA", UiWorldMapTexOffsetVCallback, map->lineUv);
}
