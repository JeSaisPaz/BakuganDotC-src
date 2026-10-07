// bdc 0x089f5600 GfxSpriteLayerCloneSprite
#include "bdc.h"

/* Adds a copy of sprite `src` to `layer`: takes a pool slot or heap-allocates a new sprite, copies
   `src` into it (`GfxSpriteCopy`) and appends it (`GfxSpriteLayerAdd`). */

GfxSprite *GfxSpriteLayerCloneSprite(GfxSpriteLayer *self, const GfxSprite *src)

{
  bool fromLow;
  GfxSprite *sprite;
  GfxSprite *dst;
  
  dst = (GfxSprite *)0x0;
  if (self->pool != (GfxSprite *)0x0) {
    dst = GfxSpriteLayerAllocPoolSlot(self);
  }
  if (dst == (GfxSprite *)0x0) {
    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    sprite = MemAlloc(0x160,(char *)0x0,0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    dst = (GfxSprite *)0x0;
    if (sprite != (GfxSprite *)0x0) {
      GfxSpriteCtor(sprite);
      dst = sprite;
    }
  }
  GfxSpriteCopy(src,dst);
  return (GfxSprite *)GfxSpriteLayerAdd(self,(CoreObject *)dst);
}

