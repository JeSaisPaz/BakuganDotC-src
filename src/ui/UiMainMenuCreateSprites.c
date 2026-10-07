// bdc 0x089a5d8c UiMainMenuCreateSprites
#include "bdc.h"

/* Builds the main menu's sprites (phase 0): restores the cursor (`UiMainMenuGetSavedCursor`),
   marks all five items unlocked (`unlockedMask`), clears the 0x500-byte tween slots, creates
   the sprites of UI layout 10 (`UiLayoutCreateSprites`) into `data[]` and hides the first 19
   (visible bit cleared, alpha 0), then allocates three extra 0x160-byte `GfxSprite`s
   `data[19..21]` as hidden copies of `data[1]` textured `NonTexture`, and centres `data[1]`'s
   pivot with scale 1 / angle 0. */

void UiMainMenuCreateSprites(UiMainMenu *self)
{
  u8 unlocked[5];
  GfxSprite **sprites;
  GfxSprite *sprite;
  GfxSprite *mem;
  bool fromLow;
  u8 mask;
  int i;

  unlocked[0] = 1;
  unlocked[1] = 1;
  unlocked[2] = 1;
  unlocked[3] = 1;
  unlocked[4] = 1;
  self->cursor = (s8)UiMainMenuGetSavedCursor();
  self->unlockedMask = 0;
  self->modelCursor = self->cursor;
  mask = self->unlockedMask;
  for (i = 0; i < 5; i++) {
    if (unlocked[i] != 0) {
      mask = (u8)(mask | (1 << i));
    }
  }
  self->unlockedMask = mask;
  memset(self->slots, 0, 0x500);
  UiLayoutCreateSprites(self->base.spriteLayer, (GfxSprite **)self->base.data, 10);

  for (i = 0; i < 19; i++) {
    ((GfxSprite **)self->base.data)[i]->flags &= ~1u;
    ((GfxSprite **)self->base.data)[i]->alpha = 0.0f;
  }

  for (i = 0; i < 3; i++) {
    sprite = NULL;
    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    mem = (GfxSprite *)MemAlloc(0x160, NULL, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    if (mem != NULL) {
      GfxSpriteCtor(mem);
      sprite = mem;
    }
    ((GfxSprite **)self->base.data)[19 + i] = sprite;
    GfxSpriteLayerAdd(self->base.spriteLayer,
                      (CoreObject *)((GfxSprite **)self->base.data)[19 + i]);
    sprites = (GfxSprite **)self->base.data;
    GfxSpriteCopy(sprites[1], sprites[19 + i]);
    ((GfxSprite **)self->base.data)[19 + i]->flags &= ~1u;
    sprite = ((GfxSprite **)self->base.data)[19 + i];
    sprite->texture = GfxFindTexture("NonTexture");
    ((GfxSprite **)self->base.data)[19 + i]->alpha = 0.0f;
  }

  GfxSpriteCenterPivot(((GfxSprite **)self->base.data)[1]);
  ((GfxSprite **)self->base.data)[1]->flags |= 0x20;
  UiSpriteSetScaleRotation(((GfxSprite **)self->base.data)[1], 1.0f, 1.0f, 0.0f);
}
