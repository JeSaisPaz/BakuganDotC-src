// bdc 0x089fe69c UiWindowFrameDtor
#include "bdc.h"

/* Destructor of a 9-slice window frame (`UiWindowFrame`, 0x130 bytes: a
   `GfxSpriteLayer` with vtable `g_uiWindowFrameVtable` at `+0x74`, plus a `CoreObject`
   at `+0x80`): restores both vtables, releases its sprites (`GfxSpriteLayerClear`), destroys the
   embedded `CoreObject`, runs `GfxSpriteLayerDtor` and frees the frame when `flags & 1`. */

void UiWindowFrameDtor(UiWindowFrame *self, u32 flags)

{
  if (self != NULL) {
    ((GfxSpriteLayer *)self)->vtbl = g_uiWindowFrameVtable;
    self->object.vtable = g_uiWindowFrameObjectVtable;
    GfxSpriteLayerClear((GfxSpriteLayer *)self);
    CoreObjectDtor(&self->object, 0);
    GfxSpriteLayerDtor((GfxSpriteLayer *)self, 0);
    if ((flags & 1) != 0) {
      MemLock();
      MemFree(self, NULL, 0);
      MemUnlock();
    }
  }
}
