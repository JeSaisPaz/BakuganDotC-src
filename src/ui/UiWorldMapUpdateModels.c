// bdc 0x08997168 UiWorldMapUpdateModels
#include "bdc.h"

/* Per-frame model update of `UiWorldMap`: if the map model `+0x1f04` exists, calls
   its update (vtable `+0x3c`) and advances the two texture scrolls (`UiWorldMapScrollLines`,
   `UiWorldMapScrollSea`); then updates the jet model `+0x226c` the same way. */

void UiWorldMapUpdateModels(UiScreen *screen)

{
  UiWorldMap *map = (UiWorldMap *)screen;
  GfxModel *model = map->mapModel;

  if (model != NULL) {
    const VtblEntry *update = &((const VtblEntry *)model->base.vtable)[7];

    ((void (*)(void *))update->fn)((u8 *)model + update->delta);
    UiWorldMapScrollLines(screen);
    UiWorldMapScrollSea(screen);
  }
  model = map->jetModel;
  if (model != NULL) {
    const VtblEntry *update = &((const VtblEntry *)model->base.vtable)[7];

    ((void (*)(void *))update->fn)((u8 *)model + update->delta);
  }
}
