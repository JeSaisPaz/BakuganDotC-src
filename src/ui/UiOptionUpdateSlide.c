// bdc 0x089705c8 UiOptionUpdateSlide
#include "bdc.h"

/* Advances the vertical slide of `UiOption` by 1/16 per frame: opening rises 288 px
   with a 16 px overshoot then settles (`UiEquipSetPendingFlag(2)` at the bounce), closing drops by 272 px;
   applies the offset to the Y of the 58 layout sprites. Returns 1 when finished (end of the settle or
   of the close), else 0. */

u8 UiOptionUpdateSlide(UiOption *self, u8 closing)
{
  u8 done;
  int i;
  float t;
  float start;
  float u;

  t = self->slideT + 0.0625f;
  done = 0;
  start = self->slideStart;
  if (closing == 0) {
    if (self->slideBounced == 0) {
      u = t - 1.0f;
      self->slideT = t;
      self->slideOffset = start - (1.0f - u * u) * 288.0f;
      if (!(t < 1.0f)) {
        self->slideT = 0.0f;
        self->slideOffset = -16.0f;
        self->slideStart = -16.0f;
        UiEquipSetPendingFlag(2);
        self->slideBounced = self->slideBounced + 1;
      }
    }
    else {
      self->slideT = t;
      self->slideOffset = start + t * t * 16.0f;
      if (!(t < 1.0f)) {
        done = 1;
        self->slideOffset = 0.0f;
        self->slideStart = 0.0f;
      }
    }
  }
  else {
    self->slideT = t;
    self->slideOffset = start + t * t * 272.0f;
    if (!(t < 1.0f)) {
      self->slideOffset = 272.0f;
      done = 1;
    }
  }
  for (i = 0; i < 0x3a; i++) {
    ((GfxSprite **)self->base.data)[i]->posY = self->spriteY[i] + self->slideOffset;
  }
  return done != 0;
}
