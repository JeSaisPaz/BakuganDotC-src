// bdc 0x089a9e28 UiMainMenuStepItemSlide
#include "bdc.h"

/* Without `closing`: steps the five item sprites `data[5..9]` with their tweens `slots[5..9]`.
   After each one's `delay0b` runs out, leg 1 (toggle07 0) eases x as slideStart + t^2*slideDelta
   (t += 1/8); at t >= 1 it parks the sprite at x 704 with scale 1, then primes leg 2 (toggle07 1,
   t 0, delay 10 frames, slideStart = x, slideDelta = x - target, target `itemHomeX` for the cursor
   item and -224 for the others). Leg 2 eases x as slideStart - (1-(t-1)^2)*slideDelta
   (t += 1/16); at t >= 1 it snaps to the target and tints the sprite white (item unlocked in
   `unlockedMask`) or grey 0.6, alpha 1. Returns 1 when all five are at t >= 1 of leg 2, else 0.
   With `closing`: advances the cursor item's tween t by 1/8, fades its alpha (startAlpha - t^2) and
   grows its scale (startScale + t^2); at t >= 1 hides it (clears flag 1) and returns 1, else 0. */

int UiMainMenuStepItemSlide(UiMainMenu *self, u8 closing)

{
  UiTween *tween;
  GfxSprite *sprite;
  int i;
  u8 done;
  float t;
  float u;

  done = 0;
  if (closing == 0) {
    for (i = 5; i < 10; i++) {
      tween = &self->slots[i].tween;
      if (tween->delay0b != 0) {
        tween->delay0b--;
        continue;
      }
      if (tween->toggle07 == 0) {
        t = tween->t + 0.125f;
        tween->t = t;
        ((GfxSprite **)self->base.data)[i]->posX =
            (float)tween->slideStart + t * t * (float)tween->slideDelta;
        if (tween->t < 1.0f)
          continue;
        ((GfxSprite **)self->base.data)[i]->posX = 704.0f;
        ((GfxSprite **)self->base.data)[i]->scaleX = 1.0f;
        ((GfxSprite **)self->base.data)[i]->scaleY = 1.0f;
        sprite = ((GfxSprite **)self->base.data)[i];
        GfxSpriteSetScaleRotation(sprite, sprite->scaleX, sprite->scaleY, sprite->angle, false);
        tween->toggle07 = 1;
        tween->t = 0.0f;
        tween->delay0b = 10;
        tween->slideStart = (s16)(int)((GfxSprite **)self->base.data)[i]->posX;
        if (self->cursor == i - 5)
          tween->slideDelta =
              (s16)(int)(((GfxSprite **)self->base.data)[i]->posX - self->itemHomeX);
        else
          tween->slideDelta = (s16)(int)(((GfxSprite **)self->base.data)[i]->posX - -224.0f);
      }
      else {
        t = tween->t + 0.0625f;
        u = t - 1.0f;
        tween->t = t;
        ((GfxSprite **)self->base.data)[i]->posX =
            (float)tween->slideStart - (1.0f - u * u) * (float)tween->slideDelta;
        if (tween->t < 1.0f)
          continue;
        if (self->cursor == i - 5)
          ((GfxSprite **)self->base.data)[i]->posX = self->itemHomeX;
        else
          ((GfxSprite **)self->base.data)[i]->posX = -224.0f;
        sprite = ((GfxSprite **)self->base.data)[i];
        if ((self->unlockedMask & (1 << (i - 5))) != 0) {
          sprite->tint[0] = 1.0f;
          sprite->tint[1] = 1.0f;
          sprite->tint[2] = 1.0f;
          sprite->alpha = 1.0f;
        }
        else {
          sprite->tint[0] = 0.6f;
          sprite->tint[1] = 0.6f;
          sprite->tint[2] = 0.6f;
          sprite->alpha = 1.0f;
        }
        done++;
      }
    }
    return done == 5;
  }
  tween = &self->slots[5 + self->cursor].tween;
  t = tween->t + 0.125f;
  tween->t = t;
  ((GfxSprite **)self->base.data)[5 + self->cursor]->alpha = tween->startAlpha - t * t;
  tween = &self->slots[5 + self->cursor].tween;
  ((GfxSprite **)self->base.data)[5 + self->cursor]->scaleX =
      tween->startScale + tween->t * tween->t;
  ((GfxSprite **)self->base.data)[5 + self->cursor]->scaleY =
      ((GfxSprite **)self->base.data)[5 + self->cursor]->scaleX;
  sprite = ((GfxSprite **)self->base.data)[5 + self->cursor];
  GfxSpriteSetScaleRotation(sprite, sprite->scaleX, sprite->scaleY, sprite->angle, false);
  if (self->slots[5 + self->cursor].tween.t < 1.0f)
    return 0;
  ((GfxSprite **)self->base.data)[5 + self->cursor]->flags &= ~1u;
  return 1;
}
