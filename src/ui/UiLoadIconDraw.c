// bdc 0x08809018 UiLoadIconDraw
#include "bdc.h"

/* Draw of the 'now loading' icon task (vtable `0x08af145c` slot 4): when the icon sprite's alpha
   (`+0x18`→`+0xbc`) is above 0, draws its sprite layer `+0x10` (`GfxSpriteLayerDraw`) into a
   new render packet at depth 21000 (`GfxNewRenderPacket`). */

void UiLoadIconDraw(CoreTask *self)

{
  UiLoadIcon *icon = (UiLoadIcon *)self;
  GfxSpriteLayer *layer;
  void *packet;

  if (!(icon->sprite->alpha <= 0.0f)) {
    layer = icon->layer;
    packet = GfxNewRenderPacket(21000.0f);
    GfxSpriteLayerDraw(layer, packet);
  }
}
