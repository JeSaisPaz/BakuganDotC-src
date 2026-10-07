// bdc 0x0898dc28 UiCollectionFigureZoomSelectedCell
#include "bdc.h"

/* Per-frame zoom-in of the selected cell of `UiCollectionFigure`: raises
   the zoom factor `+0xe80` by 0.1 up to 1 (scale 1.0 → 1.2) and applies it to the cursor sprite
   (data `+0x18`), the cell sprite and its label sprite (data `[cursor]`, `[cursor + 25]`), bringing
   them forward (Z −502/−500/−501). `UiCollectionFigureResetCursor` resets the factor. */

void UiCollectionFigureZoomSelectedCell(UiCollectionFigure *self)
{
  float pulse = self->pulse;
  float scale;

  if (pulse < 1.0f) {
    pulse = pulse + 0.1f;
    self->pulse = pulse;
  }
  scale = pulse * 0.20000005f + 1.0f;
  if (!(scale <= 1.2f)) {
    scale = 1.2f;
  }
  UiSpriteSetScaleRotation(((GfxSprite **)self->base.data)[6], scale, scale, 0.0f);
  ((GfxSprite **)self->base.data)[6]->posZ = -502.0f;
  UiSpriteSetScaleRotation(((GfxSprite **)self->base.data)[self->cursor], scale, scale, 0.0f);
  ((GfxSprite **)self->base.data)[self->cursor]->posZ = -500.0f;
  UiSpriteSetScaleRotation(((GfxSprite **)self->base.data)[self->cursor + 25], scale, scale, 0.0f);
  ((GfxSprite **)self->base.data)[self->cursor + 25]->posZ = -501.0f;
}
