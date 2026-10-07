// bdc 0x0890a4b8 UiScreenUpdateBg
#include "bdc.h"

/* Steps the screen's own background animation list `+0x54` (`GfxFabListUpdate`) when the screen owns
   one (`+0x50` != NULL). */

void UiScreenUpdateBg(UiScreen *screen)

{
  if (screen->bgData != (void *)0x0) {
    GfxFabListUpdate(&screen->bgAnimList);
  }
  return;
}

