// bdc 0x08975474 UiCollectionMenuHideCursor
#include "bdc.h"

/* Hides the cursor sprite 0x0c and the highlight copy 0x19 of
   `UiCollectionMenu`. */

void UiCollectionMenuHideCursor(UiCollectionMenu *self)

{
  GfxSprite **sprites = (GfxSprite **)self->base.data;

  sprites[12]->flags &= ~1u;
  sprites[25]->flags &= ~1u;
  return;
}
