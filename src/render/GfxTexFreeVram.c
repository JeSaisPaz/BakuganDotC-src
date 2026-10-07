// bdc 0x089f69b0 GfxTexFreeVram
#include "bdc.h"

/* Free callback matching `GfxTexAllocVram`: if the pointer belongs to the display's VRAM
   allocator (`GfxDisplayVramOwns`) releases it there (`GfxDisplayVramFree`), otherwise `MemFreeAligned`. */

void GfxTexFreeVram(void *ptr)

{
  int owned;
  
  owned = GfxDisplayVramOwns(g_gfxDisplay,ptr);
  if (owned == 0) {
    MemFreeAligned(ptr);
  }
  else {
    GfxDisplayVramFree(g_gfxDisplay,ptr);
  }
  return;
}

