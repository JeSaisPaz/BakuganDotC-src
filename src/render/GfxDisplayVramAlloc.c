// bdc 0x089cec58 GfxDisplayVramAlloc
#include "bdc.h"

/* Allocates VRAM from the display's `Mem2Init` texture pool: calls the allocator's
   (`display->vramAlloc`, `+0x48`) virtual alloc (vtable slot `+0xc`) with the this-adjusted
   pointer. Callers pass `(display, size, 0, 0)` (`GfxTexAllocVram`). */

typedef struct VramVtblEntry {
  short thisAdjust;
  short pad;
  void *(*alloc)(void *, s32, s32, s32);
} VramVtblEntry;

typedef struct VramAllocObj {
  VramVtblEntry *vtbl;
} VramAllocObj;

void *GfxDisplayVramAlloc(GfxDisplay *display, s32 size, s32 a2, s32 a3)
{
  VramAllocObj *obj = (VramAllocObj *)display->vramAlloc;
  VramVtblEntry *e = obj->vtbl + 1;
  return e->alloc((char *)obj + e->thisAdjust, size, a2, a3);
}
