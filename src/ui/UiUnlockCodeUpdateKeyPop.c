// bdc 0x08992dec UiUnlockCodeUpdateKeyPop
#include "bdc.h"

/* Per-frame key-press pop of `UiUnlockCode` started by
   `UiUnlockCodeStartKeyPop`: for 6 frames fades the pop sprite (data `+0x84`) by 1/6 per frame
   and grows it by 1/12 of its size per frame, then clears the active flag `+0xa8`. */

void UiUnlockCodeUpdateKeyPop(UiUnlockCode *self)
{
  GfxSprite *sprite;
  float width;
  float height;
  float scale;

  if (self->keyPopOn != '\0') {
    self->keyPopTimer = self->keyPopTimer + 1;
    sprite = ((GfxSprite **)self->base.data)[0x84 / 4];
    sprite->alpha = sprite->alpha - 0.16666667f;
    scale = (float)self->keyPopTimer * 0.083333336f + 1.0f;
    width = GfxSpriteGetWidth(((GfxSprite **)self->base.data)[0x84 / 4]);
    sprite = ((GfxSprite **)self->base.data)[0x84 / 4];
    height = GfxSpriteGetHeight(sprite);
    GfxSpriteSetSize(sprite, width * scale, height * scale);
    if (5 < self->keyPopTimer) {
      self->keyPopTimer = 0;
      self->keyPopOn = '\0';
    }
  }
}
