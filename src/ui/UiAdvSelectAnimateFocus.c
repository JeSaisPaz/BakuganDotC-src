// bdc 0x08919f34 UiAdvSelectAnimateFocus
#include "bdc.h"

/* Per-frame focus animation of the adventure partner-select screen: ramps `focusZoom` by 0.1 per
   frame up to 1, derives scale = 1 + 0.2 * focusZoom (capped at 1.2), applies it to the focused
   candidate's sprites (5 + cursor, 11 + cursor, 18 + cursor) and sprite 17, brings them to the
   front (depth -200..-203), unhides sprites 11/18 + cursor (flag 0x20) and places sprite
   18 + cursor at its rest position plus the scaled candidate offset. */

void UiAdvSelectAnimateFocus(UiAdvSelect *self)
{
  float zoom;
  float scale;

  zoom = self->focusZoom;
  if (zoom < 1.0f) {
    zoom = zoom + 0.1f;
    self->focusZoom = zoom;
  }
  scale = zoom * 0.20000005f + 1.0f;
  if (!(scale <= 1.2f)) {
    scale = 1.2f;
  }

  UiSpriteSetScaleRotation(((GfxSprite **)self->base.data)[self->cursor + 5], scale, scale, 0.0f);
  ((GfxSprite **)self->base.data)[self->cursor + 5]->posZ = -200.0f;

  UiSpriteSetScaleRotation(((GfxSprite **)self->base.data)[self->cursor + 11], scale, scale, 0.0f);
  ((GfxSprite **)self->base.data)[self->cursor + 11]->flags &= ~0x20u;
  ((GfxSprite **)self->base.data)[self->cursor + 11]->posZ = -201.0f;

  UiSpriteSetScaleRotation(((GfxSprite **)self->base.data)[self->cursor + 18], scale, scale, 0.0f);
  ((GfxSprite **)self->base.data)[self->cursor + 18]->flags &= ~0x20u;
  ((GfxSprite **)self->base.data)[self->cursor + 18]->posZ = -202.0f;
  ((GfxSprite **)self->base.data)[self->cursor + 18]->posX =
      self->spritePos[self->cursor + 5][0] + self->candidateOffsetX * scale;
  ((GfxSprite **)self->base.data)[self->cursor + 18]->posY =
      self->spritePos[self->cursor + 5][1] + self->candidateOffsetY * scale;

  UiSpriteSetScaleRotation(((GfxSprite **)self->base.data)[17], scale, scale, 0.0f);
  ((GfxSprite **)self->base.data)[17]->posZ = -203.0f;
}
