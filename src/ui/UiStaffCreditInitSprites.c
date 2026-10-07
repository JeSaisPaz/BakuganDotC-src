// bdc 0x08944c50 UiStaffCreditInitSprites
#include "bdc.h"

/* Sets up the 31-entry sprite table (`data`, `GfxSprite` pointers) of the staff-credits screen
   (task 3004, `UiStaffCreditCtor`) after the layout is loaded: allocates (low heap) a new sprite
   into entry 30, adds it to the screen's sprite layer and makes it a copy of entry 24 with the same
   size at y = 288; then hides all 31 sprites (flags bit0 cleared) with tint 0.5 and alpha 1, shows
   entries 4..7, mirrors entry 5 horizontally, centres entries 6/7 (flags bit5 set) and centres the 13
   picture sprites (entries 10..22) after shifting them by `g_staffCreditPictureOffsetX` — left for
   even entries, right for odd ones — with alpha 0. */

#define UI_STAFF_CREDIT_SPRITE(screen, i) (((GfxSprite **)(screen)->data)[i])

void UiStaffCreditInitSprites(UiScreen *screen)
{
  bool fromLow;
  GfxSprite *mem;
  GfxSprite *copy;
  GfxSprite *sprite;
  float w;
  float h;
  s32 i;

  copy = NULL;
  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  mem = MemAlloc(sizeof(GfxSprite), NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  if (mem != NULL) {
    GfxSpriteCtor(mem);
    copy = mem;
  }
  UI_STAFF_CREDIT_SPRITE(screen, 30) = copy;
  GfxSpriteLayerAdd(screen->spriteLayer, (CoreObject *)UI_STAFF_CREDIT_SPRITE(screen, 30));
  GfxSpriteCopy(UI_STAFF_CREDIT_SPRITE(screen, 24), UI_STAFF_CREDIT_SPRITE(screen, 30));
  sprite = UI_STAFF_CREDIT_SPRITE(screen, 30);
  w = GfxSpriteGetWidth(UI_STAFF_CREDIT_SPRITE(screen, 24));
  h = GfxSpriteGetHeight(UI_STAFF_CREDIT_SPRITE(screen, 24));
  UiSpriteSetSize(w, h, sprite);
  UI_STAFF_CREDIT_SPRITE(screen, 30)->posY = 288.0f;

  for (i = 0; i < 31; i++) {
    UI_STAFF_CREDIT_SPRITE(screen, i)->flags &= ~1u;
    sprite = UI_STAFF_CREDIT_SPRITE(screen, i);
    sprite->tint[0] = 0.5f;
    sprite->tint[1] = 0.5f;
    sprite->tint[2] = 0.5f;
    sprite->alpha = 1.0f;
  }
  for (i = 4; i < 8; i++) {
    UI_STAFF_CREDIT_SPRITE(screen, i)->flags |= 1u;
  }
  GfxSpriteFlipU(UI_STAFF_CREDIT_SPRITE(screen, 5));
  for (i = 6; i < 8; i++) {
    GfxSpriteCenterPivot(UI_STAFF_CREDIT_SPRITE(screen, i));
    GfxSpriteResetMatrix(UI_STAFF_CREDIT_SPRITE(screen, i));
    UI_STAFF_CREDIT_SPRITE(screen, i)->flags |= 0x20u;
  }
  for (i = 10; i < 23; i++) {
    if ((i & 1) != 0) {
      UI_STAFF_CREDIT_SPRITE(screen, i)->posX += g_staffCreditPictureOffsetX;
    } else {
      UI_STAFF_CREDIT_SPRITE(screen, i)->posX -= g_staffCreditPictureOffsetX;
    }
    GfxSpriteCenterPivot(UI_STAFF_CREDIT_SPRITE(screen, i));
    GfxSpriteResetMatrix(UI_STAFF_CREDIT_SPRITE(screen, i));
    UI_STAFF_CREDIT_SPRITE(screen, i)->alpha = 0.0f;
  }
}
