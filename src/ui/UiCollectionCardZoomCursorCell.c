// bdc 0x089845f8 UiCollectionCardZoomCursorCell
#include "bdc.h"

/* Scales the cursor, the selected cell and its card art up to 1.2 (+0.1 per frame of `pulse`) in
   `UiCollectionCard` and brings them to the front. */

void UiCollectionCardZoomCursorCell(UiCollectionCard *self)
{
  float t;
  float scale;

  t = self->pulse;
  if (t < 1.0f) {
    t = t + 0.1f;
    self->pulse = t;
  }
  scale = t * 0.20000005f + 1.0f;
  if (!(scale <= 1.2f)) {
    scale = 1.2f;
  }
  UiSpriteSetScaleRotation(((GfxSprite **)self->base.data)[4], scale, scale, 0.0f);
  ((GfxSprite **)self->base.data)[4]->posZ = -502.0f;
  UiSpriteSetScaleRotation(((GfxSprite **)self->base.data)[self->cursor], scale, scale, 0.0f);
  ((GfxSprite **)self->base.data)[self->cursor]->posZ = -500.0f;
  UiSpriteSetScaleRotation(((GfxSprite **)self->base.data)[23 + self->cursor], scale, scale, 0.0f);
  ((GfxSprite **)self->base.data)[23 + self->cursor]->posZ = -501.0f;
}
