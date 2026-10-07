// bdc 0x0890a2d0 UiSharedAnimGetLength
#include "bdc.h"

/* Returns the length of the first track of the shared animation in `slot`
   (`GfxFabGetFrameCount`), or 0 when the slot is empty. */

u32 UiSharedAnimGetLength(void *owner, int slot)
{
  GfxFab *fab = g_uiSharedAnims[slot];

  if (fab != (GfxFab *)0x0) {
    return GfxFabGetFrameCount(fab);
  }
  return 0;
}
