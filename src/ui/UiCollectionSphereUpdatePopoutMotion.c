// bdc 0x0897dd34 UiCollectionSphereUpdatePopoutMotion
#include "bdc.h"

/* Updates the model in slot `slot` of `UiCollectionSphere` (vtable +0x3c)
   except on special pages. */

void UiCollectionSphereUpdatePopoutMotion(UiCollectionSphere *self, u8 slot)

{
  GfxModel *model;

  if (UiCollectionSphereGetPageKind(self, self->page) != 2) {
    model = self->models[slot];
    if (model != NULL) {
      const VtblEntry *update = &((const VtblEntry *)model->base.vtable)[7];

      ((void (*)(void *))update->fn)((u8 *)model + update->delta);
    }
  }
}
