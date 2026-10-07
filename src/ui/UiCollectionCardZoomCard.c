// bdc 0x08986e3c UiCollectionCardZoomCard
#include "bdc.h"

/* In the zoom view of `UiCollectionCard`, R/L scales the card art (±0.015
   within 0.6..1.5) and re-applies its scale (`GfxSpriteSetScaleRotation`). The card art is
   sprite 13 + cursor; the sprite pointer is re-read from the table before every access. */

void UiCollectionCardZoomCard(UiCollectionCard *self)
{
  PadState *pad = self->base.pad;
  GfxSprite *sprite = ((GfxSprite **)self->base.data)[13 + self->cursor];

  if (pad->buttons & 0x100) {
    sprite->scaleX = sprite->scaleX + 0.015f;
    sprite = ((GfxSprite **)self->base.data)[13 + self->cursor];
    if (!(sprite->scaleX <= 1.5f)) {
      sprite->scaleX = 1.5f;
      sprite = ((GfxSprite **)self->base.data)[13 + self->cursor];
    }
  } else if (pad->buttons & 0x200) {
    sprite->scaleX = sprite->scaleX - 0.015f;
    sprite = ((GfxSprite **)self->base.data)[13 + self->cursor];
    if (sprite->scaleX < 0.6f) {
      sprite->scaleX = 0.6f;
      sprite = ((GfxSprite **)self->base.data)[13 + self->cursor];
    }
  } else {
    return;
  }
  sprite->scaleY = sprite->scaleX;
  ((GfxSprite **)self->base.data)[13 + self->cursor]->angle = 0.0f;
  sprite = ((GfxSprite **)self->base.data)[13 + self->cursor];
  GfxSpriteSetScaleRotation(sprite, sprite->scaleX, sprite->scaleY, sprite->angle, false);
}
