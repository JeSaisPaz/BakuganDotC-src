// bdc 0x089296cc UiHologramViewStartBlink
#include "bdc.h"

/* Arms the blink record `blinkOn` of the hologram detail view (`UiHologramViewCtor`, task 392):
   on, and with base 0.7 for `blinkBaseA`/`blinkBaseB` when `dim` is set; no-op for view kind 5. */

void UiHologramViewStartBlink(UiHologramView *self, char dim)
{
  if (self->kind != 5) {
    memset(&self->blinkOn, 0, 0x10);
    self->blinkOn = 1;
    if (dim != 0) {
      self->blinkBaseA = 0.7f;
      self->blinkBaseB = 0.7f;
    }
  }
}
