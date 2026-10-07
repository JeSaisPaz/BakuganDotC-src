// bdc 0x0896bb50 UiCardEquipUpdateFade
#include "bdc.h"

/* Advances the fade level `+0x29e0` of `UiCardEquip` by 1/8 per frame (ease-out
   up to 0.8 when opening, ease-in down to 0 when closing). Returns 1 when finished. */

u8 UiCardEquipUpdateFade(UiCardEquip *self, u8 closing)
{
  u8 done;
  float t;
  float target;

  t = self->fadeT + 0.125f;
  done = 0;
  target = self->fadeTarget;
  if (closing == 0) {
    self->fadeT = t;
    self->fade = target + (1.0f - (t - 1.0f) * (t - 1.0f)) * 0.8f;
    if (!(t < 1.0f)) {
      self->fade = 0.8f;
      return 1;
    }
  } else {
    self->fadeT = t;
    self->fade = target - t * t * 0.8f;
    if (!(t < 1.0f)) {
      done = 1;
      self->fade = 0.0f;
    }
  }
  return done;
}
