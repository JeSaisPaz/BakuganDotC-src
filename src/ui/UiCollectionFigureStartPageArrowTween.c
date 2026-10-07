// bdc 0x0898d024 UiCollectionFigureStartPageArrowTween
#include "bdc.h"

/* Starts the tween of the page-arrow sprites 0x13..0x18 (two groups of three, records from
   `+0x36c`) of `UiCollectionFigure`. On the way in (`out` = 0) makes them
   visible, mirrors 0x16..0x18 horizontally (`GfxSpriteFlipU`), tints sprites 0x13/0x16 blue
   (0, 0.5, 1) with alpha 0, scales them 1×2 and resets the arrow dimming
   (`UiCollectionFigureSetPageArrowAlpha` with alpha 0); on the way out only restarts the
   tweens. */

void UiCollectionFigureStartPageArrowTween(UiCollectionFigure *self, u8 out)
{
  GfxSprite *sprite;
  int i;

  if (out == 0) {
    for (i = 0x13; i < 0x19; i++) {
      ((GfxSprite **)self->base.data)[i]->flags |= 1;
      sprite = ((GfxSprite **)self->base.data)[i];
      if (i >= 0x16) {
        GfxSpriteFlipU(sprite);
        sprite = ((GfxSprite **)self->base.data)[i];
      }
      if (i == 0x13 || i == 0x16) {
        sprite->tint[0] = 0.0f;
        sprite->tint[1] = 0.5f;
        sprite->tint[2] = 1.0f;
        sprite->alpha = 0.0f;
        sprite = ((GfxSprite **)self->base.data)[i];
      }
      UiTweenBegin(1.0f, out, sprite, &self->tweens[i], 3);
      UiSpriteSetScaleRotation(((GfxSprite **)self->base.data)[i], 1.0f, 2.0f, 0.0f);
    }
    UiCollectionFigureSetPageArrowAlpha(self, 0.0f);
  }
  else {
    for (i = 0x13; i < 0x19; i++) {
      UiTweenBegin(1.0f, out, ((GfxSprite **)self->base.data)[i], &self->tweens[i], 3);
    }
  }
}
