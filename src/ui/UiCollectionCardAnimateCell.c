// bdc 0x089845cc UiCollectionCardAnimateCell
#include "bdc.h"

/* Animates (`UiCursorGlowStep`) the selected card cell sprite of
   `UiCollectionCard`. */

void UiCollectionCardAnimateCell(UiCollectionCard *self)

{
  UiCursorGlowStep(((GfxSprite **)self->base.data)[self->cursor]);
  return;
}
