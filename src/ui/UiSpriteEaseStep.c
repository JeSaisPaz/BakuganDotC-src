// bdc 0x089a9ae4 UiSpriteEaseStep
#include "bdc.h"

/* Generic eased tween step for one sprite, tween record `state`: advances the phase `t` by
   `1/frames` and, per `flags`, eases alpha (1: `startAlpha` ± `alphaDelta`), uniform scale
   (2: from `scaleFrom` to `scaleTo` around `startScale`), x (4) or else y (8) position
   (`slideStart` + `slideDelta`), using `1-(t-1)^2` when opening and `t^2` when `closing`.
   Once `t >= 1` an opening tween snaps to its end values (alpha = `alphaDelta`, scale = `scaleTo`,
   position = `slideEnd`) and a closing one clears bit 0 of the sprite flags. With flag 2 the scale
   is applied through `GfxSpriteSetScaleRotation` every step. Returns 1 when finished (`t >= 1`,
   NaN included), else 0. */

int UiSpriteEaseStep(float scaleFrom, float scaleTo, float frames, float alphaDelta, u8 closing, GfxSprite *sprite, UiTween *state, u8 flags)

{
  float u;
  int done;

  done = 0;
  if (closing == 0) {
    state->t = state->t + 1.0f / frames;
    if ((flags & 1) != 0) {
      u = state->t - 1.0f;
      sprite->alpha = state->startAlpha + (1.0f - u * u) * alphaDelta;
    }
    if ((flags & 2) != 0) {
      if (!(scaleFrom <= scaleTo)) {
        u = state->t - 1.0f;
        sprite->scaleX = state->startScale - (1.0f - u * u) * (scaleFrom - scaleTo);
      }
      if (scaleFrom < scaleTo) {
        u = state->t - 1.0f;
        sprite->scaleX = state->startScale + (1.0f - u * u) * (scaleTo - scaleFrom);
      }
      sprite->scaleY = sprite->scaleX;
    }
    if ((flags & 4) != 0) {
      u = state->t - 1.0f;
      sprite->posX = (float)state->slideStart + (1.0f - u * u) * (float)state->slideDelta;
    }
    else if ((flags & 8) != 0) {
      u = state->t - 1.0f;
      sprite->posY = (float)state->slideStart + (1.0f - u * u) * (float)state->slideDelta;
    }
    if (!(state->t < 1.0f)) {
      if ((flags & 1) != 0) {
        sprite->alpha = alphaDelta;
      }
      if ((flags & 2) != 0) {
        sprite->scaleX = scaleTo;
        sprite->scaleY = scaleTo;
      }
      if ((flags & 4) != 0) {
        sprite->posX = (float)state->slideEnd;
      }
      else if ((flags & 8) != 0) {
        sprite->posY = (float)state->slideEnd;
      }
      done = 1;
    }
  }
  else {
    state->t = state->t + 1.0f / frames;
    if ((flags & 1) != 0) {
      sprite->alpha = state->startAlpha - state->t * state->t * alphaDelta;
    }
    if ((flags & 2) != 0) {
      if (!(scaleFrom <= scaleTo)) {
        sprite->scaleX = state->startScale - state->t * state->t * (scaleFrom - scaleTo);
      }
      if (scaleFrom < scaleTo) {
        sprite->scaleX = state->startScale + state->t * state->t * (scaleTo - scaleFrom);
      }
      sprite->scaleY = sprite->scaleX;
    }
    if ((flags & 4) != 0) {
      u = state->t;
      sprite->posX = (float)state->slideStart + u * u * (float)state->slideDelta;
    }
    else if ((flags & 8) != 0) {
      u = state->t;
      sprite->posY = (float)state->slideStart + u * u * (float)state->slideDelta;
    }
    if (!(state->t < 1.0f)) {
      sprite->flags = sprite->flags & ~1u;
      done = 1;
    }
  }
  if ((flags & 2) != 0) {
    GfxSpriteSetScaleRotation(sprite, sprite->scaleX, sprite->scaleY, sprite->angle, false);
  }
  return done;
}
