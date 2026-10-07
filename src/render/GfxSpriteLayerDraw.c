// bdc 0x089f5054 GfxSpriteLayerDraw
#include "bdc.h"

/* Draw method of a 2D sprite layer: forwards to `GfxSpriteLayerDraw2D``(layer, packet)`. */

void GfxSpriteLayerDraw(GfxSpriteLayer *self, void *packet)

{
  GfxSpriteLayerDraw2D(self,packet);
  return;
}

