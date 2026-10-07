// bdc 0x08909c50 UiScreenDraw
#include "bdc.h"

/* Draw (vtable slot 4) of the `UiScreen` base class: allocates a render packet at depth 2000
   (`GfxNewRenderPacket`) and, when the sprite layer `+0x18` exists, draws the layer into it
   (`GfxSpriteLayerDraw(layer, packet)`). */

void UiScreenDraw(UiScreen *screen)

{
  void *packet;
  
  packet = GfxNewRenderPacket(2000.0f);
  if (screen->spriteLayer != (GfxSpriteLayer *)0x0) {
    GfxSpriteLayerDraw(screen->spriteLayer,packet);
  }
  return;
}

