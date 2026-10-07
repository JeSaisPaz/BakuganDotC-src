// bdc 0x0881a084 UiLayoutCreateSprites
#include "bdc.h"

/* Creates the sprites of UI layout `layout` (0..63) in the sprite group `group`: the layout is the
   UiLayoutEntry array `g_uiLayoutTables[layout]` with `g_uiLayoutCounts[layout]` entries; for each
   entry it creates a sprite at `(x, y, z + 200)` with the entry's texture
   (`GfxSpriteLayerCreateSpriteByName`), sets its texture rectangle `(u, v, u + w, v + h)` and size
   `(w, h)` (`UiSpriteSetUvRect`, `UiSpriteSetSize`), shifts sprites with quad mode 1 or 3 by
   half their size and stores it in `outSprites[i]`. Returns 0 for an invalid layout id, else 1. */

int UiLayoutCreateSprites(void *group, GfxSprite **outSprites, u32 layout)

{
  const UiLayoutEntry *entry;
  GfxSprite *sprite;
  int i;
  int mode;
  float w;
  float h;
  float pos[4] __attribute__((aligned(16)));
  float uv[4];

  if ((int)layout < 0 || layout >= 0x40) {
    return 0;
  }
  entry = (const UiLayoutEntry *)g_uiLayoutTables[layout];
  for (i = 0; i < g_uiLayoutCounts[layout]; i++, entry++, outSprites++) {
    pos[0] = (float)entry->x;
    pos[1] = (float)entry->y;
    pos[2] = (float)(entry->z + 200);
    pos[3] = 0.0f;
    sprite = GfxSpriteLayerCreateSpriteByName(group, entry->texture, pos, false);
    *outSprites = sprite;
    uv[0] = (float)entry->u;
    uv[1] = (float)entry->v;
    uv[2] = uv[0] + (float)entry->w;
    uv[3] = uv[1] + (float)entry->h;
    w = (float)entry->w;
    h = (float)entry->h;
    UiSpriteSetUvRect(sprite, uv);
    UiSpriteSetSize(w, h, sprite);
    mode = sprite->quadMode;
    if (mode == 1 || mode == 3) {
      sprite->posX = sprite->posX + w * 0.5f;
      sprite->posY = sprite->posY + h * 0.5f;
    }
  }
  return 1;
}
