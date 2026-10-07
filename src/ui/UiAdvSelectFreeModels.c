// bdc 0x089178e8 UiAdvSelectFreeModels
#include "bdc.h"

/* Deletes the Bakugan model `+0x914` and the pedestal model `+0x918` of the adventure
   partner-select screen (`UiAdvSelectCtor`, task 376) through their virtual destructors
   (vtable at object `+0x14`, slot at vtable `+8`, called with flag 3), clearing both pointers. */

void UiAdvSelectFreeModels(UiAdvSelect *self)
{
  GfxModel *obj;
  const VtblEntry *slot;

  obj = (GfxModel *)self->model;
  if (obj != NULL) {
    slot = &((const VtblEntry *)obj->base.vtable)[1];
    ((void (*)(void *, s32))slot->fn)((u8 *)obj + slot->delta, 3);
    self->model = NULL;
    self->model = NULL;
  }
  obj = (GfxModel *)self->pedestal;
  if (obj != NULL) {
    slot = &((const VtblEntry *)obj->base.vtable)[1];
    ((void (*)(void *, s32))slot->fn)((u8 *)obj + slot->delta, 3);
    self->pedestal = NULL;
    self->pedestal = NULL;
  }
}
