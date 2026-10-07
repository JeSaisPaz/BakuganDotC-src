// bdc 0x089a76e8 UiMainMenuStepTitleSlide
#include "bdc.h"

/* Steps the title-sprite slide started by `UiMainMenuStartTitleSlide` (`slots[6]`, t += 1/8
   per frame) in two legs. Leg 1 (`toggle07` 0) eases the sprite out from `slideStart` toward
   `slideEnd` (down for `dir` 0, up for `dir` 1) while growing its scale; at t >= 1 it snaps to
   the end, sets scale 1.2, rebases (start scale 1.2, new end 20 px further on, t = 0) and enters
   leg 2. Leg 2 eases in by t*t and shrinks the scale; at t >= 1 it snaps to `slideEnd`, sets the
   final scale (1.0 for `dir` 0, 0.8 for `dir` 1) and the function returns 1. Every other call
   returns 0. The sprite's model matrix is rebuilt each call. */

static GfxSprite *TitleSprite(UiMainMenu *self)
{
  return ((GfxSprite **)self->base.data)[6];
}

int UiMainMenuStepTitleSlide(UiMainMenu *self, u8 dir)
{
  UiTween *tween = &self->slots[6].tween;
  u8 leg2 = tween->toggle07;
  float t = tween->t + 0.125f;
  float start = (float)tween->slideStart;
  float delta = (float)tween->slideDelta;
  int done = 0;
  GfxSprite *sprite;
  float u;

  if (dir == 0) {
    if (leg2 == 0) {
      tween->t = t;
      TitleSprite(self)->posY = start + (1.0f - (t - 1.0f) * (t - 1.0f)) * delta;
      u = tween->t - 1.0f;
      TitleSprite(self)->scaleX = tween->startScale + (1.0f - u * u) * 0.40000004f;
      TitleSprite(self)->scaleY = TitleSprite(self)->scaleX;
      sprite = TitleSprite(self);
      if (!(tween->t < 1.0f)) {
        TitleSprite(self)->posY = (float)tween->slideEnd;
        TitleSprite(self)->scaleX = 1.2f;
        TitleSprite(self)->scaleY = 1.2f;
        tween->startScale = TitleSprite(self)->scaleX;
        tween->slideEnd = (s16)(int)(TitleSprite(self)->posY + 20.0f);
        tween->t = 0.0f;
        tween->toggle07 = 1;
        tween->slideStart = (s16)(int)TitleSprite(self)->posY;
        sprite = TitleSprite(self);
      }
    } else {
      tween->t = t;
      TitleSprite(self)->posY = start + t * t * delta;
      u = tween->t;
      TitleSprite(self)->scaleX = tween->startScale - u * u * 0.20000005f;
      TitleSprite(self)->scaleY = TitleSprite(self)->scaleX;
      sprite = TitleSprite(self);
      if (!(tween->t < 1.0f)) {
        done = 1;
        TitleSprite(self)->posY = (float)tween->slideEnd;
        TitleSprite(self)->scaleX = 1.0f;
        TitleSprite(self)->scaleY = 1.0f;
        sprite = TitleSprite(self);
      }
    }
  } else {
    if (leg2 == 0) {
      tween->t = t;
      TitleSprite(self)->posY = start - (1.0f - (t - 1.0f) * (t - 1.0f)) * delta;
      u = tween->t - 1.0f;
      TitleSprite(self)->scaleX = tween->startScale + (1.0f - u * u) * 0.20000005f;
      TitleSprite(self)->scaleY = TitleSprite(self)->scaleX;
      sprite = TitleSprite(self);
      if (!(tween->t < 1.0f)) {
        TitleSprite(self)->posY = (float)tween->slideEnd;
        TitleSprite(self)->scaleX = 1.2f;
        TitleSprite(self)->scaleY = 1.2f;
        tween->startScale = TitleSprite(self)->scaleX;
        tween->slideEnd = (s16)(int)(TitleSprite(self)->posY - 20.0f);
        tween->t = 0.0f;
        tween->toggle07 = 1;
        tween->slideStart = (s16)(int)TitleSprite(self)->posY;
        sprite = TitleSprite(self);
      }
    } else {
      tween->t = t;
      TitleSprite(self)->posY = start - t * t * delta;
      u = tween->t;
      TitleSprite(self)->scaleX = tween->startScale - u * u * 0.40000004f;
      TitleSprite(self)->scaleY = TitleSprite(self)->scaleX;
      sprite = TitleSprite(self);
      if (!(tween->t < 1.0f)) {
        TitleSprite(self)->posY = (float)tween->slideEnd;
        TitleSprite(self)->scaleX = 0.8f;
        done = 1;
        TitleSprite(self)->scaleY = 0.8f;
        sprite = TitleSprite(self);
      }
    }
  }
  GfxSpriteSetScaleRotation(sprite, TitleSprite(self)->scaleX, TitleSprite(self)->scaleY,
                            TitleSprite(self)->angle, false);
  return done != 0;
}
