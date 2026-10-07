// bdc 0x0898dbd0 UiCollectionFigurePulseCursor
#include "bdc.h"

/* Per-frame glow of the cell cursor sprite (data `+0x18`) of
   `UiCollectionFigure`: runs the shared add-colour pulse `UiPulseStepTint`
   (0..0.3 over 40 frames, state record `+0x164`). */

void UiCollectionFigurePulseCursor(UiCollectionFigure *self)
{
  UiPulseStepTint(40.0f, ((GfxSprite **)self->base.data)[6], (UiPulse *)&self->tweens[6]);
}
