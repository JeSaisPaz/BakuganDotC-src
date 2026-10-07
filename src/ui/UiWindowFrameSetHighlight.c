// bdc 0x089fed54 UiWindowFrameSetHighlight
#include "bdc.h"

/* Sets the highlight flag (bit 2 of `+0x9c`) of a 9-slice window frame (`UiWindowFrame`, 0x130
   bytes: a `GfxSpriteLayer` with vtable `0x08af5954` at `+0x74`, plus a
   `CoreObject` at `+0x80`); `UiWindowFrameApplyColors` then eases it to full brightness. */

void UiWindowFrameSetHighlight(UiWindowFrame *self)

{
  self->flags = self->flags | 4;
  return;
}

