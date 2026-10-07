// bdc 0x089fe7c8 UiWindowFrameDraw
#include "bdc.h"

/* Draw (vtable slot `+0x1c`) of a 9-slice window frame (`UiWindowFrame`, 0x130 bytes: a
   `GfxSpriteLayer` with vtable `g_uiWindowFrameVtable` at `+0x74`, plus a `CoreObject`
   at `+0x80`): when visible (`+0x9c` bit 0) opens a render packet at `depth + frame->depth`
   (`+0xf0`) and draws the sprite layer (`GfxSpriteLayerDraw`). */

void UiWindowFrameDraw(float depth, UiWindowFrame *self)

{
  void *packet;
  
  if ((self->flags & 1) != 0) {
    packet = GfxNewRenderPacket(depth + self->depth);
    GfxSpriteLayerDraw((GfxSpriteLayer *)self,packet);
  }
  return;
}

