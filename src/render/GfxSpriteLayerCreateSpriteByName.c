// bdc 0x089f5454 GfxSpriteLayerCreateSpriteByName
#include "bdc.h"

/* Looks up texture `name` with `GfxFindTexture` and creates a sprite for it on `layer` at `pos`
   (`GfxSpriteLayerCreateSprite`). */

GfxSprite *GfxSpriteLayerCreateSpriteByName(GfxSpriteLayer *self, const char *name, const float *pos, bool smooth)

{
  void *texture;
  
  texture = GfxFindTexture(name);
  return GfxSpriteLayerCreateSprite(self,texture,pos,smooth);
}

