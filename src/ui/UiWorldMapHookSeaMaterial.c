// bdc 0x08998048 UiWorldMapHookSeaMaterial
#include "bdc.h"

/* Resets the UV scroll vector `seaUv` of `UiWorldMap` and registers it with the
   `"psp_sea__DS2"` material of the map model `mapModel` (`GfxModelSetMaterialAnimCallback` with callback `0x089963dc`);
   animated by `UiWorldMapScrollSea`. */

void UiWorldMapHookSeaMaterial(UiScreen *screen)
{
  UiWorldMap *map = (UiWorldMap *)screen;
  GfxModel *self = map->mapModel;

  map->seaUv[0] = 0.0f;
  map->seaUv[1] = 0.0f;
  map->seaUv[2] = 0.0f;
  map->seaUv[3] = 0.0f;
  GfxModelSetMaterialAnimCallback(self, "psp_sea__DS2", UiWorldMapTexOffsetUCallback, map->seaUv);
}
