// bdc 0x08a325dc UiWindowFrameSetAlpha
#include "bdc.h"

/* Sets the alpha `+0xfc` of a 9-slice window frame (`UiWindowFrame`, vtable `0x08af5954`) (vtable
   entry 9). */

void UiWindowFrameSetAlpha(UiWindowFrame *self, float alpha)

{
  self->alpha = alpha;
  return;
}

