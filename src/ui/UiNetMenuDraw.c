// bdc 0x0894df54 UiNetMenuDraw
#include "bdc.h"

/* Draw (vtable slot 4) of the multiplayer (ad-hoc) top menu (task id 1999): submits its sprite
   layer and overlays to render packets. */

void UiNetMenuDraw(UiScreen *screen)

{
  void *packet;

  if (screen->spriteLayer != (GfxSpriteLayer *)0x0) {
    UiScreenDrawBg(screen);
    packet = GfxNewRenderPacket(55.0f);
    GfxSpriteLayerSetLayerMask(screen->spriteLayer,1);
    GfxSpriteLayerDraw(screen->spriteLayer,packet);
    packet = GfxNewRenderPacket(200.0f);
    GfxSpriteLayerSetLayerMask(screen->spriteLayer,2);
    GfxSpriteLayerDraw(screen->spriteLayer,packet);
  }
  UiNetMenuDrawHelpText(screen);
  return;
}
