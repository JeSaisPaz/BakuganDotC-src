// bdc 0x089754d0 UiCollectionMenuAnimateButton
#include "bdc.h"

/* Animates (`UiCursorGlowStep`) the selected entry button (sprite 0x0d + selection) of
   `UiCollectionMenu`. */

void UiCollectionMenuAnimateButton(UiCollectionMenu *self)

{
  UiCursorGlowStep(((GfxSprite **)self->base.data)[13 + (&self->selMain)[self->page]]);
  return;
}
