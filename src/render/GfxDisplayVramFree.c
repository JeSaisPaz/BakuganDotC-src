// bdc 0x089cec88 GfxDisplayVramFree
#include "bdc.h"

/* Frees a VRAM block of the display: small row-padding slots (`GfxDisplayVramIsSlotAddr`) are
   released with `GfxDisplayVramFreeSlot`; otherwise, if `Mem2FindUsedBlock` knows the pointer,
   it is returned through the pool allocator's virtual free (vtable slot `+0x14`). NULL is ignored.
    */

void GfxDisplayVramFree(GfxDisplay *display, void *ptr)
{
  if (ptr != 0) {
    if (GfxDisplayVramIsSlotAddr(display, ptr) == 0) {
      if (Mem2FindUsedBlock(display->vramAlloc, ptr) != 0) {
        MemMng2 *obj = (MemMng2 *)display->vramAlloc;
        const VtblEntry *e = &obj->vtbl[2];
        ((void (*)(void *, void *))e->fn)((char *)obj + e->delta, ptr);
      }
    } else {
      GfxDisplayVramFreeSlot(display, ptr);
    }
  }
}
