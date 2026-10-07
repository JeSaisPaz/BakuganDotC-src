// bdc 0x089ce904 GfxDisplayDtor
#include "bdc.h"

/* Destructor of the `GfxDisplay` object: destroys the VRAM allocator (`Mem2Destroy`(alloc, 3))
   if present, releases the frame list and packet pools (`GfxRenderShutdown`) and frees the object
   when bit 0 of `flags` is set. */

void GfxDisplayDtor(GfxDisplay *display, u32 flags)

{
  if (display != (GfxDisplay *)0x0) {
    if (display->vramAlloc != (void *)0x0) {
      Mem2Destroy((MemMng2 *)display->vramAlloc, 3);
      display->vramAlloc = (void *)0x0;
    }
    GfxRenderShutdown();
    if ((flags & 1) != 0) {
      MemLock();
      MemFree(display,(char *)0x0,0);
      MemUnlock();
    }
  }
  return;
}

