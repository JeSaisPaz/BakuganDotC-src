// bdc 0x089f5070 GfxSpriteLayerDrawWorld
#include "bdc.h"

/* Draw method of a 3D (billboard) sprite layer: forwards its four arguments to
   `GfxSpriteLayerDraw3D`. */

void GfxSpriteLayerDrawWorld(GfxSpriteLayer *self, void *packet, void *camera, void *arg)

{
  GfxSpriteLayerDraw3D(self,packet,camera,arg);
  return;
}

