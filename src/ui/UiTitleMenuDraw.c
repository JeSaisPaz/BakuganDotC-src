// bdc 0x089503dc UiTitleMenuDraw
#include "bdc.h"

/* Draw (vtable slot 4) of the title/system menu screen (task id 1000): submits its sprite layer and
   overlays to render packets. */

void UiTitleMenuDraw(UiScreen *screen)

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

