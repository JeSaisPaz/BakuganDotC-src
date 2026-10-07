// bdc 0x0898dba0 UiCollectionFigureHideCursor
#include "bdc.h"

/* Hides the cursor sprites of `UiCollectionFigure`: clears the visible bit
   of sprite 6 (data `+0x18`, the cell cursor) and sprite 0x44 (data `+0x110`, the highlight passed
   to `UiPulseStep`). */

void UiCollectionFigureHideCursor(UiCollectionFigure *self)

{
  GfxSprite **sprites = (GfxSprite **)self->base.data;

  sprites[6]->flags &= ~1u;
  sprites[68]->flags &= ~1u;
  return;
}
