// bdc 0x08950d30 UiTitleDraw
#include "bdc.h"

/* Draw (vtable slot 4) of the UiTitle screen (task id 200): emits the sprite layer `+0x18` and its
   own primitives into render packets at depths 50; calls `GfxFabListDraw`. */

void UiTitleDraw(UiScreen *screen)

{
  void *packet;
  GfxSpriteLayer *self;
  
  packet = GfxNewRenderPacket(50.0f);
  if (screen->spriteLayer != (GfxSpriteLayer *)0x0) {
    if (screen->bgData == (void *)0x0) {
      self = screen->spriteLayer;
    }
    else {
      GfxFabListDraw(&screen->bgAnimList);
      self = screen->spriteLayer;
    }
    GfxSpriteLayerDraw(self,packet);
  }
  return;
}

