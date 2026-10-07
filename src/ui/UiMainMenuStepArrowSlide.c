// bdc 0x089aa2f4 UiMainMenuStepArrowSlide
#include "bdc.h"

/* Steps the arrow sprite slide started by `UiMainMenuStartArrowSlide` (layout sprites 3 and 4,
   tween slots 3 and 4; t += 1/8 per frame, so 8 frames). Opening (`closing` 0): alpha =
   startAlpha + ease, x eases from slideStart by slideDelta (sprite 3 leftwards, 4 rightwards); when
   done alpha = 1, x = slideEnd, z = -20. Closing: alpha = startAlpha - t^2, scale = startScale + t^2,
   x eases 64 px outwards; when done the sprite is hidden (flags bit 0 cleared). Returns 1 once both
   sprites have reached t >= 1 this frame, else 0. */

int UiMainMenuStepArrowSlide(UiMainMenu *self, u8 closing)
{
  GfxSprite *sprite;
  UiTween *tween;
  int i;
  u8 done;
  float t;
  float e;

  done = 0;
  if (closing == 0) {
    for (i = 3; i < 5; i++) {
      tween = &self->slots[i].tween;
      t = tween->t + 0.125f;
      e = t - 1.0f;
      tween->t = t;
      ((GfxSprite **)self->base.data)[i]->alpha = tween->startAlpha + (1.0f - e * e);
      sprite = ((GfxSprite **)self->base.data)[i];
      e = tween->t - 1.0f;
      if (i == 3) {
        sprite->posX = (float)tween->slideStart - (1.0f - e * e) * (float)tween->slideDelta;
      } else {
        sprite->posX = (float)tween->slideStart + (1.0f - e * e) * (float)tween->slideDelta;
      }
      if (!(tween->t < 1.0f)) {
        ((GfxSprite **)self->base.data)[i]->alpha = 1.0f;
        ((GfxSprite **)self->base.data)[i]->posX = (float)tween->slideEnd;
        done++;
        ((GfxSprite **)self->base.data)[i]->posZ = -20.0f;
      }
    }
  } else {
    for (i = 3; i < 5; i++) {
      tween = &self->slots[i].tween;
      t = tween->t + 0.125f;
      tween->t = t;
      ((GfxSprite **)self->base.data)[i]->alpha = tween->startAlpha - t * t;
      ((GfxSprite **)self->base.data)[i]->scaleX = tween->startScale + tween->t * tween->t;
      sprite = ((GfxSprite **)self->base.data)[i];
      sprite->scaleY = sprite->scaleX;
      sprite = ((GfxSprite **)self->base.data)[i];
      GfxSpriteSetScaleRotation(sprite, sprite->scaleX, sprite->scaleY, sprite->angle, false);
      sprite = ((GfxSprite **)self->base.data)[i];
      e = tween->t - 1.0f;
      if (i == 3) {
        sprite->posX = (float)tween->slideStart - (1.0f - e * e) * 64.0f;
      } else {
        sprite->posX = (float)tween->slideStart + (1.0f - e * e) * 64.0f;
      }
      if (!(tween->t < 1.0f)) {
        done++;
        ((GfxSprite **)self->base.data)[i]->flags &= ~1u;
      }
    }
  }
  return done == 2;
}
