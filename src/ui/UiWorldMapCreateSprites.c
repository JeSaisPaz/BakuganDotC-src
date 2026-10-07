// bdc 0x08997cc4 UiWorldMapCreateSprites
#include "bdc.h"

/* Creates the sprites of `UiWorldMap`: clears the 0xeb0-byte state from `+0x74`
   (`spriteTween` up to `spriteBaseZ`), builds layout 0x24 (`UiLayoutCreateSprites`, 0x5d sprites)
   hidden with alpha 0, records their Z in `spriteBaseZ[]`, centres their pivots and resets their
   scale/rotation; insets the UVs of sprites 0x32..0x35; caches the area label offsets
   (`labelAOffsetY`, `labelBOffsetX/Y`); clones sprite 0x18 into a new hidden sprite (data slot
   0x5d); puts sprites 0x1a, 0x20, 0x21, 0x1b on draw layer mask 4; picks the cell of sprite 0x19
   by mode (row 1, or 8 when `UiWorldMapIsRankMode`); caches the panel offsets
   (`UiWorldMapCachePanelOffsets`); and shifts sprites 0x23, 0x24, 0x27, 0x28 40 px right when
   `randomEnabled` is 0. */

static GfxSprite *Spr(UiScreen *screen, int slot)
{
  return ((GfxSprite **)screen->data)[slot];
}

void UiWorldMapCreateSprites(UiScreen *screen)
{
  UiWorldMap *map = (UiWorldMap *)screen;
  GfxSprite **spr;
  GfxSprite *sprite;
  bool fromLow;
  u32 i;

  memset(map->spriteTween, 0, 0xeb0);
  UiLayoutCreateSprites(screen->spriteLayer, screen->data, 0x24);
  for (i = 0; i < 0x5d; i++) {
    Spr(screen, i)->flags &= ~1u;
    Spr(screen, i)->alpha = 0.0f;
    map->spriteBaseZ[i] = Spr(screen, i)->posZ;
    GfxSpriteCenterPivot(Spr(screen, i));
    UiSpriteSetScaleRotation(Spr(screen, i), 1.0f, 1.0f, 0.0f);
  }
  for (i = 0x32; i < 0x35; i++) {
    GfxSpriteInsetUv(0.5f, Spr(screen, i));
  }
  GfxSpriteInsetUv(0.5f, Spr(screen, 0x35));

  spr = (GfxSprite **)screen->data;
  map->labelAOffsetY = spr[0]->posY - spr[0x10]->posY;
  map->labelBOffsetX = spr[0]->posX - spr[0x2a]->posX;
  map->labelBOffsetY = spr[0]->posY - spr[0x2a]->posY;

  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  sprite = MemAlloc(sizeof(GfxSprite), NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  if (sprite != NULL) {
    GfxSpriteCtor(sprite);
  }
  ((GfxSprite **)screen->data)[0x5d] = sprite;
  GfxSpriteLayerAdd(screen->spriteLayer, (CoreObject *)Spr(screen, 0x5d));
  GfxSpriteCopy(Spr(screen, 0x18), Spr(screen, 0x5d));
  Spr(screen, 0x5d)->flags &= ~1u;

  Spr(screen, 0x1a)->layerMask = 4;
  Spr(screen, 0x20)->layerMask = 4;
  Spr(screen, 0x21)->layerMask = 4;
  Spr(screen, 0x1b)->layerMask = 4;
  if (!UiWorldMapIsRankMode(screen)) {
    GfxSpriteSetCell(Spr(screen, 0x19), 0.0f, 1.0f);
  } else {
    GfxSpriteSetCell(Spr(screen, 0x19), 0.0f, 8.0f);
  }
  UiWorldMapCachePanelOffsets(screen);
  if (map->randomEnabled == 0) {
    Spr(screen, 0x23)->posX += 40.0f;
    Spr(screen, 0x24)->posX += 40.0f;
    Spr(screen, 0x27)->posX += 40.0f;
    Spr(screen, 0x28)->posX += 40.0f;
  }
}
