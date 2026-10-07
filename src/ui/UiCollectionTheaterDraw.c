// bdc 0x08987e20 UiCollectionTheaterDraw
#include "bdc.h"

/* Draw (vtable slot 4) of the UiCollectionTheater screen (task id 315): draws the screen background
   (`UiScreenDrawBg`); emits the sprite layer `+0x18` and its own primitives into render packets
   at depths 50, 200, 300. */

void UiCollectionTheaterDraw(UiScreen *screen)

{
  void *packet;
  
  if (screen->spriteLayer != (GfxSpriteLayer *)0x0) {
    UiScreenDrawBg(screen);
    packet = GfxNewRenderPacket(50.0f);
    GfxSpriteLayerSetLayerMask(screen->spriteLayer,1);
    GfxSpriteLayerDraw(screen->spriteLayer,packet);
    packet = GfxNewRenderPacket(200.0f);
    GfxSpriteLayerSetLayerMask(screen->spriteLayer,2);
    GfxSpriteLayerDraw(screen->spriteLayer,packet);
    packet = GfxNewRenderPacket(300.0f);
    GfxSpriteLayerSetLayerMask(screen->spriteLayer,4);
    GfxSpriteLayerDraw(screen->spriteLayer,packet);
  }
  return;
}

