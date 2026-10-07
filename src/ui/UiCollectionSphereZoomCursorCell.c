// bdc 0x0897b5f8 UiCollectionSphereZoomCursorCell
#include "bdc.h"

/* Grows `pulse` by 0.1 per call while it is below 1.0, then scales the cursor (sprite 6), the
   selected cell (`cursor`) and its label (`cursor + 27`) of
   `UiCollectionSphere` to `1 + pulse * 0.2` (capped at 1.2) and brings
   them to the front (posZ -502 / -500 / -501). */

void UiCollectionSphereZoomCursorCell(UiCollectionSphere *self)
{
  float pulse;
  float scale;

  pulse = self->pulse;
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
  UiSpriteSetScaleRotation(((GfxSprite **)self->base.data)[self->cursor + 27], scale, scale, 0.0f);
  ((GfxSprite **)self->base.data)[self->cursor + 27]->posZ = -501.0f;
}
