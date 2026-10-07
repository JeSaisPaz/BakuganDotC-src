// bdc 0x0899d4f0 UiWorldMapCacheScoreOffsets
#include "bdc.h"

/* Caches, for the stage-list digit sprites 0x49..0x57 of `UiWorldMap` (five per
   row), their offset from their row's plate sprite (`0x32 + (i / 5)`) in `+0x1e8c + i * 8`, so the
   digits can follow the plates. */

typedef struct WorldMapSprites {
  u8 _unk00[0xc8];
  GfxSprite *plate[23]; /* +0xc8 */
  GfxSprite *digit[15]; /* +0x124 */
} WorldMapSprites;

void UiWorldMapCacheScoreOffsets(UiScreen *screen)

{
  UiWorldMap *map = (UiWorldMap *)screen;
  WorldMapSprites *spr = (WorldMapSprites *)screen->data;
  int i;

  for (i = 0; i < 15; i++) {
    u8 row = (u8)(i / 5);
    map->scoreOffset[i][0] = spr->plate[row]->posX - spr->digit[i]->posX;
    map->scoreOffset[i][1] = spr->plate[row]->posY - spr->digit[i]->posY;
  }
}
