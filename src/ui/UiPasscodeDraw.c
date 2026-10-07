// bdc 0x0893d8e0 UiPasscodeDraw
#include "bdc.h"

/* Draw (vtable slot 4) of the passcode (symbol sequence) puzzle screen (task id 374): submits its
   sprite layer and overlays to render packets. */

void UiPasscodeDraw(UiScreen *screen)

{
  void *packet;
  
  if (screen->spriteLayer != (GfxSpriteLayer *)0x0) {
    UiScreenDrawBg(screen);
    packet = GfxNewRenderPacket(2000.0f);
    GfxSpriteLayerSetLayerMask(screen->spriteLayer,1);
    GfxSpriteLayerDraw(screen->spriteLayer,packet);
  }
  return;
}

