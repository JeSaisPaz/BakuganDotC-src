// bdc 0x089f56d0 GfxSpriteLayerCreateBillboardByName
#include "bdc.h"

/* Looks up texture `name` with `GfxFindTexture` and creates a 3D billboard sprite for it
   (`GfxSpriteLayerCreateBillboard`). */

GfxSprite *GfxSpriteLayerCreateBillboardByName(GfxSpriteLayer *self, const char *name)
{
  void *texture = GfxFindTexture(name);

  return GfxSpriteLayerCreateBillboard(self, texture);
}
