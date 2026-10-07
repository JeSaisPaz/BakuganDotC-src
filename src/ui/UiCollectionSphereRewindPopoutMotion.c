// bdc 0x0897dd94 UiCollectionSphereRewindPopoutMotion
#include "bdc.h"

/* Rewinds the motion of model `slot` of `UiCollectionSphere` by 500 frames
   (vtable +0x34 with -500) and updates it, except on special pages. */

void UiCollectionSphereRewindPopoutMotion(UiCollectionSphere *self, u8 slot)

{
  if (UiCollectionSphereGetPageKind(self,self->page) != 2) {
    GfxModel *model = self->models[slot];
    if (model != (GfxModel *)0x0) {
      const VtblEntry *vt = &((const VtblEntry *)(model->base).vtable)[6];
      ((void (*)(void *, float))vt->fn)((u8 *)model + vt->delta,-500.0f);
      model = self->models[slot];
      vt = &((const VtblEntry *)(model->base).vtable)[7];
      ((void (*)(void *))vt->fn)((u8 *)model + vt->delta);
    }
  }
  return;
}
