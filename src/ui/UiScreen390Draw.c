// bdc 0x08940560 UiScreen390Draw
#include "bdc.h"

/* Draw (vtable slot 4) of `UiScreen390`: opens a render packet with sort key 50
   (`GfxNewRenderPacket`) and, when the sprite layer `+0x18` exists, draws the screen background
   (`UiScreenDrawBg`) and the sprite layer (`GfxSpriteLayerDraw`). */

void UiScreen390Draw(UiScreen *screen)

{
  void *packet;
  
  packet = GfxNewRenderPacket(50.0f);
  if (screen->spriteLayer != (GfxSpriteLayer *)0x0) {
    UiScreenDrawBg(screen);
    GfxSpriteLayerDraw(screen->spriteLayer,packet);
  }
  return;
}

