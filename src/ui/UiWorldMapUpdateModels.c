// bdc 0x08997168 UiWorldMapUpdateModels
#include "bdc.h"

/* Per-frame model update of `UiWorldMap`: if the map model `+0x1f04` exists, calls
   its update (vtable `+0x3c`) and advances the two texture scrolls (`UiWorldMapScrollLines`,
   `UiWorldMapScrollSea`); then updates the jet model `+0x226c` the same way. */

typedef struct ModelVtbl {
  u8 _unk00[0x38];
  s16 thisAdjust; /* +0x38 */
  s16 pad3a;
  void (*update)(void *); /* +0x3c */
} ModelVtbl;

typedef struct ModelObj {
  u8 _unk00[0x14];
  ModelVtbl *vtable;
} ModelObj;

void UiWorldMapUpdateModels(UiScreen *screen)

{
  UiWorldMap *map = (UiWorldMap *)screen;
  ModelObj *model = (ModelObj *)map->mapModel;

  if (model != NULL) {
    model->vtable->update((u8 *)model + model->vtable->thisAdjust);
    UiWorldMapScrollLines(screen);
    UiWorldMapScrollSea(screen);
  }
  model = (ModelObj *)map->jetModel;
  if (model != NULL) {
    model->vtable->update((u8 *)model + model->vtable->thisAdjust);
  }
}
