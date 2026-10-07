// bdc 0x08983e5c UiCollectionCardStartArrowTween
#include "bdc.h"

/* Starts the fade tween (`UiTweenBegin`, tweens[0x11..0x16]) of the page arrow sprites 0x11..0x16
   of `UiCollectionCard`. On fade-in (`out == 0`) it first sets bit 0 of each
   sprite's flags, mirrors the right side (0x14..0x16), gives the outer ones (0x11, 0x14) a blue tint
   at alpha 0, stretches them to scale (1, 2) after starting the tween, and finally dims the limit
   arrows (`UiCollectionCardDimPageArrows`) to alpha 0. */

void UiCollectionCardStartArrowTween(UiCollectionCard *self, u8 out)
{
  GfxSprite *sprite;
  int i;

  if (out == 0) {
    for (i = 0x11; i < 0x17; i++) {
      sprite = ((GfxSprite **)self->base.data)[i];
      sprite->flags |= 1;
      sprite = ((GfxSprite **)self->base.data)[i];
      if (i >= 0x14) {
        GfxSpriteFlipU(sprite);
        sprite = ((GfxSprite **)self->base.data)[i];
      }
      if (i == 0x11 || i == 0x14) {
        sprite->tint[0] = 0.0f;
        sprite->tint[1] = 0.5f;
        sprite->tint[2] = 1.0f;
        sprite->alpha = 0.0f;
        sprite = ((GfxSprite **)self->base.data)[i];
      }
      UiTweenBegin(1.0f, out, sprite, &self->tweens[i], 3);
      UiSpriteSetScaleRotation(((GfxSprite **)self->base.data)[i], 1.0f, 2.0f, 0.0f);
    }
    UiCollectionCardDimPageArrows(0.0f, &self->base);
  } else {
    for (i = 0x11; i < 0x17; i++) {
      UiTweenBegin(1.0f, out, ((GfxSprite **)self->base.data)[i], &self->tweens[i], 3);
    }
  }
}
