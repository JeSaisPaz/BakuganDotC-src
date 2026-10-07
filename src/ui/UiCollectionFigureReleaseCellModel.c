// bdc 0x0898c4e4 UiCollectionFigureReleaseCellModel
#include "bdc.h"

/* Releases the model of cell `cell` of `UiCollectionFigure`
   (`CoreObjectDeferDelete`) and clears `+0x1208[cell]`. */

void UiCollectionFigureReleaseCellModel(UiCollectionFigure *self, u8 cell)

{
  if (self->models[cell] != (GfxModel *)0x0) {
    CoreObjectDeferDelete(&self->models[cell]->base,0);
    self->models[cell] = (GfxModel *)0x0;
  }
  return;
}

