// bdc 0x0891c434 UiHologramGalleryDraw
#include "bdc.h"

/* Draw (vtable slot 4) of the hologram gallery screen (task id 391): submits the sprite layer in
   passes 1, 2, 4 and 8. */

void UiHologramGalleryDraw(UiHologramGallery *self)

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
    packet = GfxNewRenderPacket(300.0f);
    GfxSpriteLayerSetLayerMask((self->base).spriteLayer,4);
    GfxSpriteLayerDraw((self->base).spriteLayer,packet);
    packet = GfxNewRenderPacket(400.0f);
    GfxSpriteLayerSetLayerMask((self->base).spriteLayer,8);
    GfxSpriteLayerDraw((self->base).spriteLayer,packet);
  }
  return;
}

