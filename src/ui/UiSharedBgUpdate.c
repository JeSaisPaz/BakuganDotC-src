// bdc 0x0890e148 UiSharedBgUpdate
#include "bdc.h"

/* Update of the shared-background task 320: runs `UiScreenUpdateCommon` and, unless it was
   closing and while the shared block `g_uiSharedAnims` exists, steps the shared background animation
   list `g_uiSharedAnimList` (`GfxFabListUpdate`), twice when the display runs at a frame-skip (30 fps) so the
   animation speed stays the same. */

void UiSharedBgUpdate(UiScreen *screen)

{
  u8 closing;

  closing = screen->closeRequested;
  UiScreenUpdateCommon(screen);
  if ((closing == 0) && (g_uiSharedAnims != (GfxFab **)0)) {
    if (g_gfxDisplay->frameSkip != 0) {
      GfxFabListUpdate((void **)&g_uiSharedAnimList);
    }
    GfxFabListUpdate((void **)&g_uiSharedAnimList);
  }
}
