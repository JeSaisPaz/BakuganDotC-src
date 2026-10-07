// bdc 0x0892f9f8 UiBakuganSelectTweenHeaderSprites
#include "bdc.h"

/* Starts the open (`closing` = 0) or close slide tweens of three sprite groups of
   `UiBakuganSelect` (sprites 2–5, 8–11 and 12–15, tweens `tweens[i]`).
   On open each sprite gets flag 1 and layer mask 2, is placed at its group's first Y
   (`spritePos[2|8|12][1]`, the third group 8 px higher after switching it to a top-left pivot at
   scale 1, angle 0) and slid by Y to its own layout position `spritePos[i][1]` with
   `UiTweenBeginSlide` (flags 0xb, third group 9); then its tint is set to (0.2, 1, 0) and alpha 0.
   On close the slide runs back from the layout position to the group's first Y with fade-out. */

void UiBakuganSelectTweenHeaderSprites(UiBakuganSelect *self, u8 closing)
{
  GfxSprite *sprite;
  int i;

  if (closing == 0) {
    for (i = 2; i < 6; i++) {
      float startY;

      ((GfxSprite **)self->base.data)[i]->flags |= 1;
      ((GfxSprite **)self->base.data)[i]->layerMask = 2;
      startY = self->spritePos[2][1];
      ((GfxSprite **)self->base.data)[i]->posY = startY;
      UiTweenBeginSlide(1.0f, 0.0f, self->spritePos[i][1] - startY, closing,
                        ((GfxSprite **)self->base.data)[i], &self->tweens[i], 0xb);
      sprite = ((GfxSprite **)self->base.data)[i];
      sprite->tint[0] = 0.2f;
      sprite->tint[1] = 1.0f;
      sprite->tint[2] = 0.0f;
      sprite->alpha = 0.0f;
    }
    for (i = 8; i < 12; i++) {
      float startY;

      ((GfxSprite **)self->base.data)[i]->flags |= 1;
      ((GfxSprite **)self->base.data)[i]->layerMask = 2;
      startY = self->spritePos[8][1];
      ((GfxSprite **)self->base.data)[i]->posY = startY;
      UiTweenBeginSlide(1.0f, 0.0f, self->spritePos[i][1] - startY, closing,
                        ((GfxSprite **)self->base.data)[i], &self->tweens[i], 0xb);
      sprite = ((GfxSprite **)self->base.data)[i];
      sprite->tint[0] = 0.2f;
      sprite->tint[1] = 1.0f;
      sprite->tint[2] = 0.0f;
      sprite->alpha = 0.0f;
    }
    for (i = 12; i < 16; i++) {
      ((GfxSprite **)self->base.data)[i]->flags |= 1;
      ((GfxSprite **)self->base.data)[i]->layerMask = 2;
      GfxSpriteSetTopLeftPivot(((GfxSprite **)self->base.data)[i]);
      UiSpriteSetScaleRotation(((GfxSprite **)self->base.data)[i], 1.0f, 1.0f, 0.0f);
      ((GfxSprite **)self->base.data)[i]->posY = self->spritePos[12][1];
      ((GfxSprite **)self->base.data)[i]->posY -= 8.0f;
      UiTweenBeginSlide(1.0f, 0.0f, self->spritePos[i][1] - self->spritePos[12][1], closing,
                        ((GfxSprite **)self->base.data)[i], &self->tweens[i], 9);
      sprite = ((GfxSprite **)self->base.data)[i];
      sprite->tint[0] = 0.2f;
      sprite->tint[1] = 1.0f;
      sprite->tint[2] = 0.0f;
      sprite->alpha = 0.0f;
    }
  }
  else {
    for (i = 2; i < 6; i++) {
      UiTweenBeginSlide(1.0f, 0.0f, self->spritePos[2][1] - self->spritePos[i][1], closing,
                        ((GfxSprite **)self->base.data)[i], &self->tweens[i], 0xb);
    }
    for (i = 8; i < 12; i++) {
      UiTweenBeginSlide(1.0f, 0.0f, self->spritePos[8][1] - self->spritePos[i][1], closing,
                        ((GfxSprite **)self->base.data)[i], &self->tweens[i], 0xb);
    }
    for (i = 12; i < 16; i++) {
      UiTweenBeginSlide(1.0f, 0.0f, self->spritePos[12][1] - self->spritePos[i][1], closing,
                        ((GfxSprite **)self->base.data)[i], &self->tweens[i], 9);
    }
  }
}
