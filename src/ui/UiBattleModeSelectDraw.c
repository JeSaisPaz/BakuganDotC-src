// bdc 0x089aff1c UiBattleModeSelectDraw
#include "bdc.h"

/* Draw (vtable slot 4) of the UiBattleModeSelect screen (task id 350): draws the screen background
   (`UiScreenDrawBg`); emits the sprite layer `+0x18` and its own primitives into render packets
   at depths 50, 200. */

void UiBattleModeSelectDraw(UiBattleModeSelect *self)

{
  void *packet;
  
  if ((self->base).spriteLayer != (GfxSpriteLayer *)0x0) {
    UiScreenDrawBg(&self->base);
    packet = GfxNewRenderPacket(50.0f);
    GfxSpriteLayerSetLayerMask((self->base).spriteLayer,1);
    GfxSpriteLayerDraw((self->base).spriteLayer,packet);
    packet = GfxNewRenderPacket(200.0f);
    GfxSpriteLayerSetLayerMask((self->base).spriteLayer,2);
    GfxSpriteLayerDraw((self->base).spriteLayer,packet);
  }
  return;
}

