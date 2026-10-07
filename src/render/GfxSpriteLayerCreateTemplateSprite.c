// bdc 0x089f5888 GfxSpriteLayerCreateTemplateSprite
#include "bdc.h"

/* Creates a sprite for `texture` on `layer` using the shared byte vertex template (quad mode 0,
   `billboardMode` 0) at vec4 `pos`: pool slot or heap allocation as in
   `GfxSpriteLayerCreateSprite`, then `GfxSpriteLayerAdd`, whose return (the sprite) it passes on. */

GfxSprite *GfxSpriteLayerCreateTemplateSprite(GfxSpriteLayer *self, void *texture, const float *pos)

{
  bool fromLow;
  GfxSprite *sprite;
  GfxSprite *sprite_00;
  
  sprite_00 = (GfxSprite *)0x0;
  if (self->pool != (GfxSprite *)0x0) {
    sprite_00 = GfxSpriteLayerAllocPoolSlot(self);
  }
  if (sprite_00 == (GfxSprite *)0x0) {
    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    sprite = MemAlloc(0x160,(char *)0x0,0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    sprite_00 = (GfxSprite *)0x0;
    if (sprite != (GfxSprite *)0x0) {
      GfxSpriteCtor(sprite);
      sprite_00 = sprite;
    }
  }
  else {
    GfxSpriteInit(sprite_00);
  }
  sprite_00->texture = texture;
  GfxSpriteSetQuadMode(sprite_00,0);
  sprite_00->billboardMode = 0;
  sprite_00->posX = pos[0];
  sprite_00->posY = pos[1];
  sprite_00->posZ = pos[2];
  sprite_00->posW = pos[3];
  return (GfxSprite *)GfxSpriteLayerAdd(self,(CoreObject *)sprite_00);
}

