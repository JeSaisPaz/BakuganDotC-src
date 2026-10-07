// bdc 0x089f4fb0 GfxSpriteLayerDtor
#include "bdc.h"

/* Destructor of a sprite layer (vtable `0x08af5854` slot 1): restores the vtable, releases all
   sprites (`GfxSpriteLayerClear`), frees the sprite pool (`+8`) and, when `flags & 1`, the layer
   itself. */

void GfxSpriteLayerDtor(GfxSpriteLayer *self, u32 flags)

{
  GfxSprite *ptr;
  
  if (self != (GfxSpriteLayer *)0x0) {
    self->vtbl = (const VtblEntry *)&g_gfxSpriteLayerVtbl;
    GfxSpriteLayerClear(self);
    ptr = self->pool;
    if (ptr != (GfxSprite *)0x0) {
      MemLock();
      MemFree(ptr,(char *)0x0,0);
      MemUnlock();
    }
    if ((flags & 1) != 0) {
      MemLock();
      MemFree(self,(char *)0x0,0);
      MemUnlock();
    }
  }
  return;
}

