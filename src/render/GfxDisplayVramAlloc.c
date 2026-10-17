// bdc 0x089cec58 GfxDisplayVramAlloc
#include "bdc.h"

/* Allocates VRAM from the display's `Mem2Init` texture pool: calls the allocator's
   (`display->vramAlloc`, `+0x48`) virtual alloc (vtable slot `+0xc`) with the this-adjusted
   pointer. Callers pass `(display, size, NULL, 0)` (`GfxTexAllocVram`): the `Mem2Alloc`
   source file/line debug arguments. */

void *GfxDisplayVramAlloc(GfxDisplay *display, u32 size, const char *file, s32 line)
{
  MemMng2 *obj = (MemMng2 *)display->vramAlloc;
  const VtblEntry *e = &obj->vtbl[1];
  return ((void *(*)(void *, u32, const char *, s32))e->fn)((char *)obj + e->delta, size, file, line);
}
