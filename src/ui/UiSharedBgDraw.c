// bdc 0x0890e1a4 UiSharedBgDraw
#include "bdc.h"

/* Draw of the shared-background task 320: sets the global 2D offset from `g_gfxVecZero`
   (`GfxScreenCameraSetViewOffset`) and draws the shared background animation list `g_uiSharedAnimList` (`GfxFabListDraw`)
   while `g_uiSharedAnims` exists. */

void UiSharedBgDraw(UiScreen *screen)

{
  GfxScreenCameraSetViewOffset((float *)&g_gfxVecZero);
  if (g_uiSharedAnims != (GfxFab **)0) {
    GfxFabListDraw(&g_uiSharedAnimList);
  }
  return;
}

