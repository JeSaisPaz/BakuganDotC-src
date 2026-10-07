// bdc 0x089ced10 GfxDisplayVramOwns
#include "bdc.h"

/* Returns 1 when `ptr` belongs to the display's VRAM allocators: inside the pool `[vramPoolStart,
   vramEnd)` (`+0x38`/`+0x3c`) or a row-padding slot (`GfxDisplayVramIsSlotAddr`); else 0. Used by
   `GfxTexFreeVram` to choose between VRAM and heap free. */

int GfxDisplayVramOwns(GfxDisplay *display, void *ptr)
{
  if (ptr < display->vramPoolStart || display->vramEnd <= ptr) {
    return GfxDisplayVramIsSlotAddr(display, ptr) != 0;
  }
  return 1;
}
