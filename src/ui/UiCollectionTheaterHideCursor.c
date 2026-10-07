// bdc 0x08989900 UiCollectionTheaterHideCursor
#include "bdc.h"

/* Hides the cursor sprite 6 and the highlight copy 0x30 of
   `UiCollectionTheater`. */

void UiCollectionTheaterHideCursor(UiScreen *screen)

{
  GfxSprite **sprites = (GfxSprite **)screen->data;

  sprites[6]->flags &= ~1u;
  sprites[48]->flags &= ~1u;
  return;
}
