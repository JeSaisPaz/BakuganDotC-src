// bdc 0x08929e58 UiHologramViewResetPages
#include "bdc.h"

/* Resets the page sequencer of the hologram detail view (`UiHologramViewCtor`, task 392; view
   kind `+0x485`) (step `+0x708`, page `+0x709`). */

void UiHologramViewResetPages(UiHologramView *self)

{
  self->step = '\0';
  self->tipPage = '\0';
  return;
}

