// bdc 0x0890a4e0 UiScreenDrawBg
#include "bdc.h"

/* Draws the screen's own background: first sets a global 2D offset from the vector at
   `g_gfxVecZero` (`GfxScreenCameraSetViewOffset`, writes `x - 0.02`, `y - 0.02` into
   `g_gfxScreenCamera + 0xc0/0xc4`), then draws the animation list `+0x54` (`GfxFabListDraw`) when
   `+0x50` != NULL. */

void UiScreenDrawBg(UiScreen *screen)

{
  GfxScreenCameraSetViewOffset(&g_gfxVecZero.x);
  if (screen->bgData != (void *)0x0) {
    GfxFabListDraw(&screen->bgAnimList);
  }
  return;
}

