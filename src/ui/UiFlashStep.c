// bdc 0x089a5080 UiFlashStep
#include "bdc.h"

/* Steps flash slot `slot` (g_uiFlashSlots). Phase 0 ramps t up by 1/frames per call, writing
   t*rgb into the sprite's addColor (alpha 1); when t reaches 1 (or is NaN) it clamps to 1 and
   moves to phase 1, returning 0. Phase 1 ramps t back down; while t > 0 it returns 0, and once
   t <= 0 it clamps to 0, clears the colour, moves to phase 2 and returns 1. Phase 2 and later
   return 1 without touching anything. */

int UiFlashStep(u8 slot)
{
  UiFlashSlot *s = &g_uiFlashSlots[slot];
  GfxSprite *sprite;
  float r, g, b, t;

  if (s->phase == 0) {
    t = s->t + 1.0f / s->frames;
    r = (float)s->r;
    g = (float)s->g;
    sprite = s->sprite;
    b = (float)s->b;
    s->t = t;
    if (!(t < 1.0f)) {
      t = 1.0f;
      s->t = 1.0f;
      s->phase = s->phase + 1;
    }
  } else {
    if (s->phase >= 2) {
      return 1;
    }
    t = s->t - 1.0f / s->frames;
    r = (float)s->r;
    g = (float)s->g;
    sprite = s->sprite;
    b = (float)s->b;
    s->t = t;
    if (t <= 0.0f) {
      s->t = 0.0f;
      sprite->addColor[3] = 1.0f;
      sprite->addColor[0] = 0.0f * r;
      sprite->addColor[1] = 0.0f * g;
      sprite->addColor[2] = 0.0f * b;
      s->phase = s->phase + 1;
      return 1;
    }
  }
  sprite->addColor[3] = 1.0f;
  sprite->addColor[0] = t * r;
  sprite->addColor[1] = t * g;
  sprite->addColor[2] = t * b;
  return 0;
}
