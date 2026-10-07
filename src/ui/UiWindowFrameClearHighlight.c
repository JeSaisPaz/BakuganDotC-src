// bdc 0x089fed64 UiWindowFrameClearHighlight
#include "bdc.h"

/* Clears the highlight flag (bit 2 of `+0x9c`) of a 9-slice window frame (`UiWindowFrame`, vtable
   `0x08af5954`); counterpart of `UiWindowFrameSetHighlight` (vtable entry 12). */

void UiWindowFrameClearHighlight(UiWindowFrame *self)

{
  self->flags = self->flags & 0xfffffffb;
  return;
}

