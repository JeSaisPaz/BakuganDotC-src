// bdc 0x089fed28 UiWindowFrameSetFillTexture
#include "bdc.h"

/* Sets the fill texture and slot on the fill sprite (`+0xac`) of a 9-slice window frame
   (`UiWindowFrame`, 0x130 bytes: a `GfxSpriteLayer` with vtable `0x08af5954`
   at `+0x74`, plus a `CoreObject` at `+0x80`) and re-lays it out (`UiWindowFrameLayoutFill`).
    */

void UiWindowFrameSetFillTexture(UiWindowFrame *self, void *tex, int slot)

{
  self->fillTexture = tex;
  self->fill->texture = tex;
  self->fill->textureSlot = slot;
  UiWindowFrameLayoutFill(self);
  return;
}

