// bdc 0x0891ca8c UiHologramGalleryInitSprites
#include "bdc.h"

/* Initial sprite setup of the hologram gallery screen (task 391, `maybe_UiScreen391Ctor`, class
   prefix `UiHologramGallery`; `fix_*` sprites, `DWHologramHelp` texts; main update `UiHologramGalleryMainPhase`
   with step `+0x2c`; placed holograms are save-profile bytes `+0x84 + slot`): clears the tweens
   `+0x7c..+0x1e04`, builds the sprites from layout 0x3c (`UiLayoutCreateSprites`), hides the 188
   layout sprites (flag bit0 cleared, alpha 0) with centred pivots and unit scale while recording
   their depth (`spriteZ`), insets the UVs of several sprite groups by half a texel, creates the
   extra sprite 188 (`GfxSpriteCtor`/`GfxSpriteLayerAdd`, a hidden copy of sprite 182) and
   measures the panels (`UiHologramGalleryMeasureNamePanel`, `UiHologramGalleryMeasureInfoPanel`,
   `UiHologramGalleryMeasureCountPanel`, `UiHologramGalleryRecordSlotX`,
   `UiHologramGalleryInitScrollArrows`). */

#define SPRITES(self) ((GfxSprite **)(self)->base.data)

void UiHologramGalleryInitSprites(UiHologramGallery *self)
{
  bool fromLow;
  GfxSprite *sprite;
  GfxSprite *extra;
  u32 i;

  memset(self->tweens, 0, 0x1d88);
  UiLayoutCreateSprites(self->base.spriteLayer, (GfxSprite **)self->base.data, 0x3c);
  for (i = 0; i < 0xbc; i++) {
    SPRITES(self)[i]->flags &= ~1u;
    SPRITES(self)[i]->alpha = 0.0f;
    GfxSpriteCenterPivot(SPRITES(self)[i]);
    UiSpriteSetScaleRotation(SPRITES(self)[i], 1.0f, 1.0f, 0.0f);
    self->spriteZ[i] = SPRITES(self)[i]->posZ;
  }
  for (i = 0xb4; i < 0xb6; i++) GfxSpriteInsetUv(0.5f, SPRITES(self)[i]);
  for (i = 0xb7; i < 0xb9; i++) GfxSpriteInsetUv(0.5f, SPRITES(self)[i]);
  for (i = 0xb6; i < 0xb7; i++) GfxSpriteInsetUv(0.5f, SPRITES(self)[i]);
  for (i = 0x86; i < 0x8c; i++) GfxSpriteInsetUv(0.5f, SPRITES(self)[i]);
  for (i = 0x8d; i < 0x93; i++) GfxSpriteInsetUv(0.5f, SPRITES(self)[i]);
  for (i = 0x96; i < 0x9c; i++) GfxSpriteInsetUv(0.5f, SPRITES(self)[i]);
  for (i = 0x8c; i < 0x8d; i++) GfxSpriteInsetUv(0.5f, SPRITES(self)[i]);
  for (i = 0x70; i < 0x74; i++) GfxSpriteInsetUv(0.5f, SPRITES(self)[i]);

  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  sprite = MemAlloc(sizeof(GfxSprite), NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  extra = NULL;
  if (sprite != NULL) {
    GfxSpriteCtor(sprite);
    extra = sprite;
  }
  SPRITES(self)[0xbc] = extra;
  GfxSpriteLayerAdd(self->base.spriteLayer, (CoreObject *)SPRITES(self)[0xbc]);
  GfxSpriteCopy(SPRITES(self)[0xb6], SPRITES(self)[0xbc]);
  SPRITES(self)[0xbc]->flags &= ~1u;
  UiHologramGalleryMeasureNamePanel(self);
  UiHologramGalleryMeasureInfoPanel(self);
  UiHologramGalleryMeasureCountPanel(self);
  UiHologramGalleryRecordSlotX(self);
  UiHologramGalleryInitScrollArrows(self);
}
