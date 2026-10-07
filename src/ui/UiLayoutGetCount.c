// bdc 0x08819ee8 UiLayoutGetCount
#include "bdc.h"

/* Returns the number of entries of UI layout `layout` (0..63) from the count table `0x08a50868`, or
   0 for an invalid id. See `UiLayoutCreateSprites`. */

s32 UiLayoutGetCount(s32 layout)

{
  s32 count;
  
  count = 0;
  if ((-1 < layout) && ((u32)layout < 0x40)) {
    count = g_uiLayoutCounts[layout];
  }
  return count;
}

