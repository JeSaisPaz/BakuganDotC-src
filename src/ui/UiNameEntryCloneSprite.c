// bdc 0x088045c8 UiNameEntryCloneSprite
#include "bdc.h"

/* Allocates a new 0x160-byte `GfxSprite` (low heap, constructed with `GfxSpriteCtor`), adds it to
   sprite layer `layer` (`GfxSpriteLayerAdd`), copies `src` into it (`GfxSpriteCopy`) and clears
   its visible bit (`+0xd0` bit 0). Returns the new sprite. Used by `UiNameEntrySetupPhase` of
   `UiNameEntry`. */

GfxSprite * UiNameEntryCloneSprite(GfxSprite *src, void *layer)

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
  dst->flags = dst->flags & 0xfffffffe;
  return dst;
}

