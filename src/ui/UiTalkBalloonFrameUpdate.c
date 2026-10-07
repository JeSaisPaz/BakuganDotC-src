// bdc 0x088c7e88 UiTalkBalloonFrameUpdate
#include "bdc.h"

/* Kind 1 update of a talk balloon sprite (`UiTalkBalloonSprite`) (the balloon frame). Always
   returns 0.
   - State 0: on the first call (`angle` (scale w) still 0) stores the move target in
     `scaleX..scaleZ`: the owner's `targetPos` (all four lanes), or, when its x/y offset from
     `homePos` is longer than 120 px, `homePos` + that x/y offset scaled to 120 px (z = homePos z),
     and sets `angle` to 1. Then runs one pop-in step (`GameFieldJiggleScaleStep`; going to
     state 1 when it finishes), or, with `g_talkBalloonSkipAnim` set, sets alpha 1 and steps
     until it finishes. Adds 0.2 to alpha (capped at 1) and moves the printer origin (all four
     lanes) to `scale + (homePos − scale) · maybe_sizeW`.
   - State 1: rounds the printer origin x/y/z to the nearest whole pixel (ties to even, VFPU
     `vf2in`/`vi2f`), w kept.
   - State 2: fades out by 0.15 per frame; at alpha below 0 clamps it to 0 and goes to state 3.
   - Other states: nothing. */

/* vf2in + vi2f: round to nearest, ties to even (pixel coordinates, well inside the int range). */
static inline float RoundHalfEven(float x)
{
  s32 i = (s32)x;
  float frac = x - (float)i;

  if (frac > 0.5f || (frac == 0.5f && (i & 1) != 0)) {
    i++;
  } else if (frac < -0.5f || (frac == -0.5f && (i & 1) != 0)) {
    i--;
  }
  return (float)i;
}

s32 UiTalkBalloonFrameUpdate(GfxSprite *sprite)
{
  UiTalkBalloonSprite *self = (UiTalkBalloonSprite *)sprite;
  UiTalkBalloon *owner;
  ScePspFVector4 *origin;
  s32 state;
  float len;
  float k;
  float t;
  float dx;
  float dy;
  float dz;
  float dw;

  state = self->state;
  if (state >= 2) {
    if (state < 3) {
      sprite->alpha = sprite->alpha - 0.15f;
      if (sprite->alpha < 0.0f) {
        sprite->alpha = 0.0f;
        self->state = 3;
      }
    }
    return 0;
  }
  if (state < 0) {
    return 0;
  }
  if (state > 0) {
    origin = &self->owner->printer->layer.view.w;
    origin->x = RoundHalfEven(origin->x);
    origin->y = RoundHalfEven(origin->y);
    origin->z = RoundHalfEven(origin->z);
    return 0;
  }

  if (sprite->angle == 0.0f) {
    owner = self->owner;
    /* scale = targetPos - homePos (xyz), w = targetPos w; then z cleared */
    sprite->scaleX = owner->targetPos[0] - owner->homePos[0];
    sprite->scaleY = owner->targetPos[1] - owner->homePos[1];
    sprite->scaleZ = owner->targetPos[2] - owner->homePos[2];
    sprite->angle = owner->targetPos[3];
    sprite->scaleZ = 0.0f;
    len = __builtin_sqrtf(sprite->scaleX * sprite->scaleX + sprite->scaleY * sprite->scaleY +
                          sprite->scaleZ * sprite->scaleZ);
    if (len <= 120.0f) {
      owner = self->owner;
      sprite->scaleX = owner->targetPos[0];
      sprite->scaleY = owner->targetPos[1];
      sprite->scaleZ = owner->targetPos[2];
      sprite->angle = owner->targetPos[3];
      sprite->angle = 1.0f;
    } else {
      /* scale = homePos + normalize(scale) * 120 (xyz); w = homePos w, then 1 */
      t = sprite->scaleX * sprite->scaleX + sprite->scaleY * sprite->scaleY +
          sprite->scaleZ * sprite->scaleZ;
      k = VfRsq(t);
      if (t == 0.0f) {
        k = 0.0f; /* bank S713; unreachable here: the offset is longer than 120 */
      }
      k = k * 120.0f;
      sprite->scaleX = sprite->scaleX * k;
      sprite->scaleY = sprite->scaleY * k;
      sprite->scaleZ = sprite->scaleZ * k;
      sprite->angle = 0.0f; /* lane w of C710: bank S713 */
      owner = self->owner;
      sprite->scaleX = owner->homePos[0] + sprite->scaleX;
      sprite->scaleY = owner->homePos[1] + sprite->scaleY;
      sprite->scaleZ = owner->homePos[2] + sprite->scaleZ;
      sprite->angle = owner->homePos[3];
      sprite->angle = 1.0f;
    }
  }

  if (g_talkBalloonSkipAnim != 0) {
    sprite->alpha = 1.0f;
    while (self->state != 1) {
      if (GameFieldJiggleScaleStep(sprite->matrix, &sprite->width, &self->phase, &self->frame,
                                   &sprite->maybe_sizeW) != 0) {
        self->state = 1;
      }
    }
  } else if (GameFieldJiggleScaleStep(sprite->matrix, &sprite->width, &self->phase, &self->frame,
                                      &sprite->maybe_sizeW) != 0) {
    self->state = 1;
  }
  sprite->alpha = sprite->alpha + 0.2f;
  owner = self->owner;
  origin = &owner->printer->layer.view.w;
  /* printer origin = scale + (homePos - scale) * maybe_sizeW, all four lanes */
  t = sprite->maybe_sizeW;
  dx = (owner->homePos[0] - sprite->scaleX) * t;
  dy = (owner->homePos[1] - sprite->scaleY) * t;
  dz = (owner->homePos[2] - sprite->scaleZ) * t;
  dw = (owner->homePos[3] - sprite->angle) * t;
  origin->x = sprite->scaleX + dx;
  origin->y = sprite->scaleY + dy;
  origin->z = sprite->scaleZ + dz;
  origin->w = sprite->angle + dw;
  if (!(sprite->alpha <= 1.0f)) {
    sprite->alpha = 1.0f;
  }
  return 0;
}
