// bdc 0x08952710 UiBattleRuleSelectDraw
#include "bdc.h"

/* Draw (vtable slot 4) of the UiBattleRuleSelect screen (task id 340): draws the screen background
   (`UiScreenDrawBg`); emits the sprite layer `+0x18` and its own primitives into render packets
   at depths 50, 200. */

void UiBattleRuleSelectDraw(UiBattleRuleSelect *self)

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

