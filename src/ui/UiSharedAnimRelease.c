// bdc 0x0890a234 UiSharedAnimRelease
#include "bdc.h"

/* Releases the shared animation in `slot` (moves the player to the deferred-free list with
   `GfxDeferredDelete`) and clears the slot. */

void UiSharedAnimRelease(void *owner, int slot)
{
  if (g_uiSharedAnims[slot] != (GfxFab *)0x0) {
    GfxDeferredDelete((CoreObject *)g_uiSharedAnims[slot]);
    g_uiSharedAnims[slot] = (GfxFab *)0x0;
  }
}
