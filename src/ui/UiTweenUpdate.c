// bdc 0x089a9160 UiTweenUpdate
#include "bdc.h"

/* Advances a UI sprite appear/disappear transition by one frame (`t += 1/frames`) and applies the
   eased alpha, scale and X/Y slide selected by `flags`; returns true once the transition has
   finished (`!(t < 1)`). */

bool UiTweenUpdate(float fromScale, float toScale, float frames, u8 fadeOut, GfxSprite *sprite, UiTween *tween, u8 flags)
{
  bool done;
  float e;

  if (fadeOut == 0) {
    /* appear: ease-out e = 1 - (t-1)^2 */
    tween->t = tween->t + 1.0f / frames;
    if ((flags & 1) != 0) {
      e = tween->t - 1.0f;
      sprite->alpha = tween->startAlpha + (1.0f - e * e);
    }
    if ((flags & 2) != 0) {
      if (!(fromScale <= toScale)) {
        e = tween->t - 1.0f;
        sprite->scaleX = tween->startScale - (1.0f - e * e) * (fromScale - toScale);
      }
      if (fromScale < toScale) {
        e = tween->t - 1.0f;
        sprite->scaleX = tween->startScale + (1.0f - e * e) * (toScale - fromScale);
      }
      sprite->scaleY = sprite->scaleX;
    }
    if ((flags & 4) != 0) {
      e = tween->t - 1.0f;
      sprite->posX = (float)tween->slideStart + (1.0f - e * e) * (float)tween->slideDelta;
    }
    else if ((flags & 8) != 0) {
      e = tween->t - 1.0f;
      sprite->posY = (float)tween->slideStart + (1.0f - e * e) * (float)tween->slideDelta;
    }
    done = false;
    if (!(tween->t < 1.0f)) {
      if ((flags & 1) != 0) {
        sprite->alpha = 1.0f;
      }
      if ((flags & 2) != 0) {
        sprite->scaleX = toScale;
        sprite->scaleY = toScale;
      }
      if ((flags & 4) != 0) {
        sprite->posX = (float)tween->slideEnd;
      }
      else if ((flags & 8) != 0) {
        sprite->posY = (float)tween->slideEnd;
      }
      done = true;
    }
  }
  else {
    /* disappear: ease-in e = t^2 */
    tween->t = tween->t + 1.0f / frames;
    if ((flags & 1) != 0) {
      sprite->alpha = tween->startAlpha - tween->t * tween->t;
    }
    if ((flags & 2) != 0) {
      if (!(fromScale <= toScale)) {
        sprite->scaleX = tween->startScale - tween->t * tween->t * (fromScale - toScale);
      }
      if (fromScale < toScale) {
        sprite->scaleX = tween->startScale + tween->t * tween->t * (toScale - fromScale);
      }
      sprite->scaleY = sprite->scaleX;
    }
    if ((flags & 4) != 0) {
      e = tween->t;
      sprite->posX = (float)tween->slideStart + e * e * (float)tween->slideDelta;
    }
    else if ((flags & 8) != 0) {
      e = tween->t;
      sprite->posY = (float)tween->slideStart + e * e * (float)tween->slideDelta;
    }
    done = false;
    if (!(tween->t < 1.0f)) {
      sprite->flags &= ~1u;
      done = true;
    }
  }
  if ((flags & 2) != 0) {
    GfxSpriteSetScaleRotation(sprite, sprite->scaleX, sprite->scaleY, sprite->angle, false);
  }
  return done;
}
