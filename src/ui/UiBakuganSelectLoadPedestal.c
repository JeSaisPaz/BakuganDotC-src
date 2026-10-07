// bdc 0x0892e9b0 UiBakuganSelectLoadPedestal
#include "bdc.h"

/* Loads the pedestal model `menu_daiza.gmo` of the Bakugan select screen (`UiBakuganSelectCtor`):
   allocates a 0x140-byte `GfxModel` from the low end of the heap, constructs it
   (`GfxModelCtor`) and stores it in `pedestal` (NULL if the allocation failed), sets a grey
   specular (colour 0.6/0.6/0.6/1.0, power 8, `GfxModelSetSpecular`), scales its root matrix to
   0.45, sets `pos` x = 0, y = -40 and copies `pos` into the root matrix translation row with w forced to 1.0. */

void UiBakuganSelectLoadPedestal(UiBakuganSelect *self)
{
  float colour[4];
  bool fromLow;
  GfxModel *alloc;
  GfxModel *model;
  GfxModel *ped;

  model = NULL;
  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  alloc = MemAlloc(sizeof(GfxModel), NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  if (alloc != NULL) {
    GfxModelCtor(alloc, "menu_daiza.gmo", 0);
    model = alloc;
  }
  self->pedestal = model;
  colour[0] = 0.6f;
  colour[1] = 0.6f;
  colour[2] = 0.6f;
  colour[3] = 1.0f;
  GfxModelSetSpecular(8.0f, model, colour, NULL);
  ped = self->pedestal;
  ped->data->rootMatrix[10] = 0.45f;
  ped->data->rootMatrix[5] = 0.45f;
  ped->data->rootMatrix[0] = 0.45f;
  ped = self->pedestal;
  ped->pos[0] = 0.0f;
  ped = self->pedestal;
  ped->pos[1] = -40.0f;
  ped = self->pedestal;
  ped->data->rootMatrix[12] = ped->pos[0];
  ped->data->rootMatrix[13] = ped->pos[1];
  ped->data->rootMatrix[14] = ped->pos[2];
  ped->data->rootMatrix[15] = ped->pos[3];
  ped = self->pedestal;
  ped->data->rootMatrix[15] = 1.0f;
}
