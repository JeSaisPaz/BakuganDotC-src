// bdc 0x08984570 UiCollectionCardHideCursor
#include "bdc.h"

/* Hides the cursor sprite 4 and the highlight copy 0x41 of
   `UiCollectionCard`. */

void UiCollectionCardHideCursor(UiCollectionCard *self)

{
  GfxSprite **sprites = (GfxSprite **)self->base.data;

  sprites[4]->flags &= ~1u;
  sprites[65]->flags &= ~1u;
  return;
}
