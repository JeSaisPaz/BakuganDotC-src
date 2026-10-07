// bdc 0x089f3abc GfxSpriteDtor
#include "bdc.h"

/* Destructor of a `GfxSprite` (vtable `0x08af583c` slot 1): resets the vtable, runs
   `CoreObjectDtor` and frees it when `flags & 1`. Also the base destructor of the effect object
   (`GfxEffectDtor`). */

void GfxSpriteDtor(GfxSprite *sprite, u32 flags)

{
  if (sprite != (GfxSprite *)0x0) {
    sprite->vtable = (void *)&g_gfxSpriteVtbl;
    CoreObjectDtor((CoreObject *)sprite,0);
    if ((flags & 1) != 0) {
      MemLock();
      MemFree(sprite,(char *)0x0,0);
      MemUnlock();
    }
  }
  return;
}

