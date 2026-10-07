// bdc 0x0898dbfc UiCollectionFigurePulseSelectedCell
#include "bdc.h"

/* Per-frame glow of the selected cell's sprite (data `[cursor +0xe78]`) of
   `UiCollectionFigure` through the shared global add-colour pulse
   `UiCursorGlowStep`. */

void UiCollectionFigurePulseSelectedCell(UiCollectionFigure *self)

{
  UiCursorGlowStep(((GfxSprite **)self->base.data)[self->cursor]);
  return;
}
