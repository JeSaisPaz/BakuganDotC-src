// bdc 0x089cec58 GfxDisplayVramAlloc
#include "bdc.h"

/* Allocates VRAM from the display's `Mem2Init` texture pool: calls the allocator's
   (`display->vramAlloc`, `+0x48`) virtual alloc (vtable slot `+0xc`) with the this-adjusted
   pointer. Callers pass `(display, size, 0, 0)` (`GfxTexAllocVram`). */

void *GfxDisplayVramAlloc(GfxDisplay *display, s32 size, s32 a2, s32 a3)
{
  MemMng2 *obj = (MemMng2 *)display->vramAlloc;
  const VtblEntry *e = &obj->vtbl[1];
  return ((void *(*)(void *, s32, s32, s32))e->fn)((char *)obj + e->delta, size, a2, a3);
}
