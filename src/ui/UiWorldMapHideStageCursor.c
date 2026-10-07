// bdc 0x0899d578 UiWorldMapHideStageCursor
#include "bdc.h"

/* Hides the stage-list cursor sprite 0x35 (data `+0xd4`) of `UiWorldMap` and the
   highlight clone (data `+0x174`). */

void UiWorldMapHideStageCursor(UiScreen *screen)
{
  GfxSprite **sprites = (GfxSprite **)screen->data;

  sprites[0x35]->flags &= ~1u;
  sprites[0x5d]->flags &= ~1u;
}
