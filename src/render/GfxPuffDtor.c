// bdc 0x08828a1c GfxPuffDtor
#include "bdc.h"

/* Destructor of the sprite puff (`GfxPuffCtor`) (vtable `g_gfxPuffVtbl` slot 1): restores the
   vtable, runs `GfxSpriteDtor` and frees the object when `flags & 1`. */

void GfxPuffDtor(GfxPuff *puff, u32 flags)

{
  if (puff != NULL) {
    puff->base.vtable = g_gfxPuffVtbl;
    GfxSpriteDtor(&puff->base, 0);
    if ((flags & 1) != 0) {
      MemLock();
      MemFree(puff, NULL, 0);
      MemUnlock();
    }
  }
  return;
}
