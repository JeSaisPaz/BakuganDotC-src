// bdc 0x0899a520 UiWorldMapPlaceAreaLabels
#include "bdc.h"

/* Runs `UiWorldMapPlaceAreaLabel` for each of the 8 area buttons of `UiWorldMap`
   that is visible. */

void UiWorldMapPlaceAreaLabels(UiScreen *screen)

{
  GfxSprite **sprites;
  int i;

  sprites = (GfxSprite **)screen->data;
  for (i = 0; i < 8; i++) {
    if ((sprites[i]->flags & 1) != 0) {
      UiWorldMapPlaceAreaLabel(screen, (u8)i);
    }
  }
}
