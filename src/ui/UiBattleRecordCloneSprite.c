// bdc 0x08948464 UiBattleRecordCloneSprite
#include "bdc.h"

/* Allocates a 0x160-byte `GfxSprite` from low memory (`GfxSpriteCtor`), adds it to `layer`
   (`GfxSpriteLayerAdd`), copies `src` into it (`GfxSpriteCopy`) and returns it. */

GfxSprite *UiBattleRecordCloneSprite(GfxSprite *src, void *layer)

{
  bool fromLow;
  GfxSprite *sprite;
  GfxSprite *dst;
  
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
  GfxSpriteLayerAdd(layer,(CoreObject *)dst);
  GfxSpriteCopy(src,dst);
  return dst;
}

