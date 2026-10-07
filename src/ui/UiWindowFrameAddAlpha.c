// bdc 0x08a325e4 UiWindowFrameAddAlpha
#include "bdc.h"

/* Adds `delta` to the alpha `+0xfc` of a 9-slice window frame (`UiWindowFrame`, vtable
   `0x08af5954`) (vtable entry 10); no clamping. */

void UiWindowFrameAddAlpha(UiWindowFrame *self, float delta)

{
  self->alpha = self->alpha + delta;
  return;
}

