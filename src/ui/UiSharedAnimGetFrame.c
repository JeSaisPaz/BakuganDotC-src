// bdc 0x0890a288 UiSharedAnimGetFrame
#include "bdc.h"

/* Returns the current frame of the first track of the shared animation in `slot`
   (`GfxFabGetFrame`), or 0 when the slot is empty. */

u32 UiSharedAnimGetFrame(void *owner, int slot)
{
  GfxFab *fab = g_uiSharedAnims[slot];

  if (fab != (GfxFab *)0x0) {
    return GfxFabGetFrame(fab);
  }
  return 0;
}
