// bdc 0x089ed328 GfxRectBaseDtor
#include "bdc.h"

/* Destructor of the base overlay rect: resets the vtable and frees the object when `flags & 1`. */

void GfxRectBaseDtor(void *rect, u32 flags)
{
  GfxRect *r = (GfxRect *)rect;

  if (r != NULL) {
    r->vtbl = g_gfxRectBaseVtbl;
    if ((flags & 1) != 0) {
      MemLock();
      MemFree(rect, NULL, 0);
      MemUnlock();
    }
  }
}
