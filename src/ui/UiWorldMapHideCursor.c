// bdc 0x0899b790 UiWorldMapHideCursor
#include "bdc.h"

/* Hides the area cursor sprite 0x18 (data `+0x60`) of `UiWorldMap` and its
   highlight clone (data `+0x174`). */

void UiWorldMapHideCursor(UiScreen *screen)
{
  GfxSprite **sprites = (GfxSprite **)screen->data;

  sprites[0x18]->flags &= ~1u;
  sprites[0x5d]->flags &= ~1u;
}
