// bdc 0x089575c4 UiEquipFreeModels
#include "bdc.h"

/* Deletes the four Bakugan models `bakuganModels` and pedestal models `pedestalModels` of
   `UiEquip` (virtual dtor, flag 3) and frees every loaded Gmo motion
   (`GmoMotionFreeAll`). */

void UiEquipFreeModels(UiEquip *self)

{
  int i;

  for (i = 0; i < 4; i++) {
    GfxModel *model = self->bakuganModels[i];
    if (model != NULL) {
      const VtblEntry *dtor = &((const VtblEntry *)model->base.vtable)[1];
      ((void (*)(void *, int))dtor->fn)((u8 *)model + dtor->delta, 3);
      self->bakuganModels[i] = NULL;
    }
    model = self->pedestalModels[i];
    if (model != NULL) {
      const VtblEntry *dtor = &((const VtblEntry *)model->base.vtable)[1];
      ((void (*)(void *, int))dtor->fn)((u8 *)model + dtor->delta, 3);
      self->pedestalModels[i] = NULL;
    }
  }
  GmoMotionFreeAll(GmoMotionMgrGet(), 0);
}
