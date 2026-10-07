// bdc 0x0897b5cc UiCollectionSphereAnimateCell
#include "bdc.h"

/* Animates (`UiCursorGlowStep`) the selected cell sprite of
   `UiCollectionSphere`. */

void UiCollectionSphereAnimateCell(UiCollectionSphere *self)

{
  UiCursorGlowStep(((GfxSprite **)self->base.data)[self->cursor]);
  return;
}

