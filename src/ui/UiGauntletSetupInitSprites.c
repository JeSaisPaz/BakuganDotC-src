// bdc 0x089328bc UiGauntletSetupInitSprites
#include "bdc.h"

/* Initial sprite setup of the gauntlet setup screen (task 373, `maybe_UiScreen373Ctor`, class
   prefix `UiGauntletSetup`; card sprites `cc_card_L_%03d`, `c_set_OK_bo_*`, texts
   `DWCardName`/`DWCardHelp`; main update `UiGauntletSetupMainPhase`; focus area `+0x74`, item `+0x76`): builds
   the sprites from the screen's layout table (`UiLayoutCreateSprites`), hides them with centred pivots and
   unit scale while recording base positions, and creates the extra 0x160-byte sprite
   (`GfxSpriteCtor`/`GfxSpriteLayerAdd`/`GfxSpriteCopy`). */

void UiGauntletSetupInitSprites(UiGauntletSetup *self)
{
  bool fromLow;
  GfxSprite *mem;
  GfxSprite *sprite;
  GfxSprite **sprites;
  u32 i;

  memset(self->tweens, 0, sizeof(self->tweens));
  UiLayoutCreateSprites(self->base.spriteLayer, self->base.data, 0x30);
  for (i = 0; i < 59; i++) {
    ((GfxSprite **)self->base.data)[i]->flags &= ~1u;
    GfxSpriteCenterPivot(((GfxSprite **)self->base.data)[i]);
    UiSpriteSetScaleRotation(((GfxSprite **)self->base.data)[i], 1.0f, 1.0f, 0.0f);
    ((GfxSprite **)self->base.data)[i]->alpha = 0.0f;
    sprites = (GfxSprite **)self->base.data;
    self->spriteZ[i] = sprites[i]->posZ;
    self->spritePos[i][0] = sprites[i]->posX;
    self->spritePos[i][1] = sprites[i]->posY;
  }
  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  mem = MemAlloc(0x160, NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  sprite = NULL;
  if (mem != NULL) {
    GfxSpriteCtor(mem);
    sprite = mem;
  }
  ((GfxSprite **)self->base.data)[59] = sprite;
  GfxSpriteLayerAdd(self->base.spriteLayer, (CoreObject *)((GfxSprite **)self->base.data)[59]);
  sprites = (GfxSprite **)self->base.data;
  GfxSpriteCopy(sprites[18], sprites[59]);
  ((GfxSprite **)self->base.data)[59]->flags &= ~1u;
  sprites = (GfxSprite **)self->base.data;
  /* slot offsets relative to sprite 7 (X/Y pairs), then Y offsets relative to sprite 14 */
  self->slotOffsets[0] = sprites[8]->posX - sprites[7]->posX;
  self->slotOffsets[1] = sprites[8]->posY - sprites[7]->posY;
  self->slotOffsets[2] = sprites[9]->posX - sprites[7]->posX;
  self->slotOffsets[3] = sprites[9]->posY - sprites[7]->posY;
  self->slotOffsets[4] = sprites[48]->posX - sprites[7]->posX;
  self->slotOffsets[5] = sprites[48]->posY - sprites[7]->posY;
  self->slotOffsets[6] = sprites[49]->posX - sprites[7]->posX;
  self->slotOffsets[7] = sprites[49]->posY - sprites[7]->posY;
  self->slotOffsets[8] = sprites[40]->posX - sprites[7]->posX;
  self->slotOffsets[9] = sprites[40]->posY - sprites[7]->posY;
  self->slotOffsets[10] = sprites[41]->posX - sprites[7]->posX;
  self->slotOffsets[11] = sprites[41]->posY - sprites[7]->posY;
  self->slotOffsets[12] = sprites[34]->posY - sprites[14]->posY;
  self->slotOffsets[13] = sprites[50]->posY - sprites[14]->posY;
  self->slotOffsets[14] = sprites[20]->posY - sprites[14]->posY;
  self->slotOffsets[15] = sprites[24]->posY - sprites[14]->posY;
}
