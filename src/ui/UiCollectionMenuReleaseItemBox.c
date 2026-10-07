// bdc 0x08975af8 UiCollectionMenuReleaseItemBox
#include "bdc.h"

/* Releases the item-box model of `UiCollectionMenu`
   (`CoreObjectDeferDelete`) before a collection screen opens and clears `+0x51c`. */

void UiCollectionMenuReleaseItemBox(UiCollectionMenu *self)

{
  if (self->itemBox != (GfxModel *)0x0) {
    CoreObjectDeferDelete(&self->itemBox->base,0);
    self->itemBox = (GfxModel *)0x0;
  }
  return;
}

