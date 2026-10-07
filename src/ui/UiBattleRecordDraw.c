// bdc 0x089488ac UiBattleRecordDraw
#include "bdc.h"

/* Draw method (vtable slot 4) of `UiBattleRecord`: draws the background
   animation list when present (`GfxFabListDraw(+0x54)`), then the sprite layer twice: pass 1 at depth
   30 and pass 2 at depth 10 (`GfxSpriteLayerSetLayerMask` selects the pass, `GfxSpriteLayerDraw`). */

void UiBattleRecordDraw(UiBattleRecord *self)

{
  void *packet;
  GfxSpriteLayer *layer;
  
  if ((self->base).bgData != (void *)0x0) {
    GfxFabListDraw(&(self->base).bgAnimList);
  }
  layer = (self->base).spriteLayer;
  if (layer != (GfxSpriteLayer *)0x0) {
    GfxSpriteLayerSetLayerMask(layer,1);
    layer = (self->base).spriteLayer;
    packet = GfxNewRenderPacket(30.0f);
    GfxSpriteLayerDraw(layer,packet);
    GfxSpriteLayerSetLayerMask((self->base).spriteLayer,2);
    layer = (self->base).spriteLayer;
    packet = GfxNewRenderPacket(10.0f);
    GfxSpriteLayerDraw(layer,packet);
  }
  return;
}

