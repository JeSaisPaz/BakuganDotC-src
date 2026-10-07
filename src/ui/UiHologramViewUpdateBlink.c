// bdc 0x0892974c UiHologramViewUpdateBlink
#include "bdc.h"

/* Steps the blink of the hologram detail view (`UiHologramViewCtor`, task 392; view kind
   `+0x485`) (`t` `+0x494` += 1/16): eases `+0x48c` up by 0.7 (`dim == 0`) or down; returns 1 when
   finished (always for kind 5). */

u8 UiHologramViewUpdateBlink(UiHologramView *self, char dim)
{
  u8 done = 0;
  float t;

  if (self->kind == 5) {
    return 1;
  }
  t = self->blinkT + 0.0625f;
  if (dim == 0) {
    self->blinkT = t;
    self->blinkBaseA = self->blinkBaseB + (1.0f - (t - 1.0f) * (t - 1.0f)) * 0.7f;
    if (!(t < 1.0f)) {
      self->blinkBaseA = 0.7f;
      return 1;
    }
  } else {
    self->blinkT = t;
    self->blinkBaseA = self->blinkBaseB - t * t * 0.7f;
    if (!(t < 1.0f)) {
      done = 1;
      self->blinkBaseA = 0.0f;
    }
  }
  return done;
}
