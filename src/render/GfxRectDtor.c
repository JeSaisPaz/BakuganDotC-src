// bdc 0x089ed70c GfxRectDtor
#include "bdc.h"

/* Destructor of the overlay rect (vtable `0x08af5734` slot 1): chains to `GfxRectBaseDtor` and
   frees it when `flags & 1`. */

void GfxRectDtor(void *rect, u32 flags)
{
  GfxRect *r = (GfxRect *)rect;

  if (r != NULL) {
    r->vtbl = g_gfxRectVtbl;
    GfxRectBaseDtor(rect, 0);
    if ((flags & 1) != 0) {
      MemLock();
      MemFree(rect, NULL, 0);
      MemUnlock();
    }
  }
}
