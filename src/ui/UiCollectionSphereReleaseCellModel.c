// bdc 0x0897b530 UiCollectionSphereReleaseCellModel
#include "bdc.h"

/* Releases the model of cell `cell` of `UiCollectionSphere`
   (`CoreObjectDeferDelete`) and clears `+0x12a4[cell]`. */

void UiCollectionSphereReleaseCellModel(UiCollectionSphere *self, u8 cell)

{
  if (self->models[cell] != (GfxModel *)0x0) {
    CoreObjectDeferDelete(&self->models[cell]->base,0);
    self->models[cell] = (GfxModel *)0x0;
  }
  return;
}

