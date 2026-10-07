// bdc 0x08939b18 UiUnlockResultBeginPop
#include "bdc.h"

/* Initialises the pop record of sprite `index` of `UiUnlockResult`
   (`tweens[index]`: t = 0, start alpha, start scale) for `UiUnlockResultPopSprite`; when opening
   (`closing` = 0) it first centres the pivot, enables linear filtering (flag 0x20) and sets
   scale 1.5. */

void UiUnlockResultBeginPop(UiUnlockResult *self, u8 closing, u8 index)
{
  GfxSprite **sprites = (GfxSprite **)self->base.data;
  GfxSprite *sprite;

  if (closing == 0) {
    GfxSpriteCenterPivot(sprites[index]);
    sprite = ((GfxSprite **)self->base.data)[index];
    sprite->flags = sprite->flags | 0x20;
    UiSpriteSetScaleRotation(((GfxSprite **)self->base.data)[index], 1.5f, 1.5f, 0.0f);
    sprite = ((GfxSprite **)self->base.data)[index];
    self->tweens[index].t = 0.0f;
    self->tweens[index].startAlpha = sprite->alpha;
    self->tweens[index].startScale = sprite->scaleX;
  } else {
    self->tweens[index].t = 0.0f;
    self->tweens[index].startAlpha = sprites[index]->alpha;
    self->tweens[index].startScale = sprites[index]->scaleX;
  }
}
