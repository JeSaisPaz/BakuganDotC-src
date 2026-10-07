// bdc 0x089f6914 GfxTexAllocVram
#include "bdc.h"

/* Allocation callback for the model library's VRAM pool: when VRAM texture placement is enabled
   (`g_gfxTexVramEnabled`) tries the display's VRAM allocator (`GfxDisplayVramAlloc(g_gfxDisplay, size, 0, 0)`),
   otherwise or on failure falls back to the aligned heap allocator `MemAllocAligned(size, 1)`. */

void *GfxTexAllocVram(s32 size)

{
  void *mem;
  
  mem = (void *)0x0;
  if (g_gfxTexVramEnabled != 0) {
    mem = GfxDisplayVramAlloc(g_gfxDisplay,size,0,0);
  }
  if (mem == (void *)0x0) {
    mem = MemAllocAligned(size,true);
    return mem;
  }
  return mem;
}

