// bdc 0x089986bc UiWorldMapCreateJetModel
#include "bdc.h"

/* Loads Marucho's jet `"fz_maruchojet_02.gmo"` for `UiWorldMap` (`GfxModelCtor`
   → `jetModel`, allocated from the low end of the heap) with lighting on, specular 0.4 grey
   (power 8), position Y -20 copied into the root matrix translation, ambient alpha 0, root matrix
   scale 0.6, plays its motion at speed 1.5 (vtable slot `+0x30`) and sets `jetScale` to 0.6. */

void UiWorldMapCreateJetModel(UiScreen *screen)
{
  UiWorldMap *map = (UiWorldMap *)screen;
  GfxModel *model;
  GfxModel *alloc;
  bool fromLow;
  float specular[4];
  const VtblEntry *entry;

  model = NULL;
  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  alloc = (GfxModel *)MemAlloc(0x140, NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  if (alloc != NULL) {
    GfxModelCtor(alloc, "fz_maruchojet_02.gmo", 0);
    model = alloc;
  }
  /* no NULL check: a failed allocation faults here */
  map->jetModel = model;
  model->lighting = 1;
  specular[0] = 0.4f;
  specular[1] = 0.4f;
  specular[2] = 0.4f;
  specular[3] = 1.0f;
  GfxModelSetSpecular(8.0f, map->jetModel, specular, NULL);
  map->jetModel->pos[1] = -20.0f;
  model = map->jetModel;
  /* root matrix translation row = pos (lv.q/sv.q copy; GfxModelSetMotionSpeed reads no VFPU) */
  model->data->rootMatrix[12] = model->pos[0];
  model->data->rootMatrix[13] = model->pos[1];
  model->data->rootMatrix[14] = model->pos[2];
  model->data->rootMatrix[15] = model->pos[3];
  map->jetModel->data->rootMatrix[15] = 1.0f;
  map->jetModel->ambient[3] = 0.0f;
  map->jetModel->data->rootMatrix[10] = 0.6f;
  map->jetModel->data->rootMatrix[5] = 0.6f;
  map->jetModel->data->rootMatrix[0] = 0.6f;
  model = map->jetModel;
  entry = &((const VtblEntry *)model->base.vtable)[6]; /* +0x30 */
  ((void (*)(void *, float))entry->fn)((u8 *)model + entry->delta, 1.5f);
  map->jetScale = 0.6f;
}
