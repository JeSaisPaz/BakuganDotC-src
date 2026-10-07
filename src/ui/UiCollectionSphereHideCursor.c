// bdc 0x0897b570 UiCollectionSphereHideCursor
#include "bdc.h"

/* Hides the cursor sprite 6 and the highlight copy 0x46 of
   `UiCollectionSphere`. */

void UiCollectionSphereHideCursor(UiCollectionSphere *self)

{
  GfxSprite **sprites = (GfxSprite **)self->base.data;

  sprites[6]->flags &= ~1u;
  sprites[70]->flags &= ~1u;
  return;
}
