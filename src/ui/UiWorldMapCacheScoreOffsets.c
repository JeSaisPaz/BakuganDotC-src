// bdc 0x0899d4f0 UiWorldMapCacheScoreOffsets
#include "bdc.h"

/* Caches, for the stage-list digit sprites 0x49..0x57 of `UiWorldMap` (five per
   row), their offset from their row's plate sprite (`0x32 + (i / 5)`) in `+0x1e8c + i * 8`, so the
   digits can follow the plates. */

void UiWorldMapCacheScoreOffsets(UiScreen *screen)

{
  UiWorldMap *map = (UiWorldMap *)screen;
  GfxSprite **spr = (GfxSprite **)screen->data;
  int i;

  for (i = 0; i < 15; i++) {
    u8 row = (u8)(i / 5);
    map->scoreOffset[i][0] = (spr + 0x32)[row]->posX - (spr + 0x49)[i]->posX;
    map->scoreOffset[i][1] = (spr + 0x32)[row]->posY - (spr + 0x49)[i]->posY;
  }
}
