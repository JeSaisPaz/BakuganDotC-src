// bdc 0x089cedf8 GfxDisplayVramFreeSlot
#include "bdc.h"

/* Releases a row-padding VRAM slot allocated by `GfxDisplayVramAllocSlot`: computes the row `(ptr
   - edram - 0x780) / 0x800` and clears its bit in `GfxDisplay``.bitmap` if it is in range
   0..0x21f. */

void GfxDisplayVramFreeSlot(GfxDisplay *display, void *ptr)
{
  int row = (int)((char *)ptr - (char *)sceGeEdramGetAddr() - 0x780) / 0x800;
  u32 bit;

  if (row >= 0 && row < 0x220) {
    bit = 1u << (row & 0x1f);
    if ((display->bitmap[row >> 5] & bit) != 0) {
      display->bitmap[row >> 5] &= ~bit;
    }
  }
}
