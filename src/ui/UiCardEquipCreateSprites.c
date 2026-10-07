// bdc 0x08969a88 UiCardEquipCreateSprites
#include "bdc.h"

/* Builds the sprites of `UiCardEquip`: clears the 0x28-byte tween records
   (`pulses`, 0x2580 bytes), creates layout 0x18 (fewer than 3 Bakugan, 0x68 sprites) or 0x19 (0xc6
   sprites) with `UiLayoutCreateSprites`, hides them all with alpha 0 and saves their depths in
   `spriteDepth`; then allocates one extra 0x160-byte `GfxSprite` as sprite `highlightSprite` (=
   the layout's sprite count; NULL when the low-heap allocation fails), adds it to the layer, copies
   sprite 11/19 (byte offset 0x2c/0x4c in `data`) into it (`GfxSpriteCopy`) as the cursor highlight and hides it. */

void UiCardEquipCreateSprites(UiCardEquip *self)
{
  GfxSprite *sprite;
  bool fromLow;
  u32 count;
  u32 i;
  int source;

  memset(self->pulses, 0, 0x2580);
  if (self->bakuganCount < 3) {
    UiLayoutCreateSprites(self->base.spriteLayer, (GfxSprite **)self->base.data, 0x18);
    count = 0x68;
    self->highlightSprite = 0x68;
    source = 11;
  }
  else {
    UiLayoutCreateSprites(self->base.spriteLayer, (GfxSprite **)self->base.data, 0x19);
    count = 0xc6;
    self->highlightSprite = 0xc6;
    source = 19;
  }

  for (i = 0; i < count; i++) {
    ((GfxSprite **)self->base.data)[i]->flags &= ~1u;
    ((GfxSprite **)self->base.data)[i]->alpha = 0.0f;
    self->spriteDepth[i] = ((GfxSprite **)self->base.data)[i]->posZ;
  }

  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  sprite = (GfxSprite *)MemAlloc(0x160, NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  if (sprite != NULL) {
    GfxSpriteCtor(sprite);
  }
  ((GfxSprite **)self->base.data)[self->highlightSprite] = sprite;
  GfxSpriteLayerAdd(self->base.spriteLayer,
                    (CoreObject *)((GfxSprite **)self->base.data)[self->highlightSprite]);
  GfxSpriteCopy(((GfxSprite **)self->base.data)[source],
                ((GfxSprite **)self->base.data)[self->highlightSprite]);
  ((GfxSprite **)self->base.data)[self->highlightSprite]->flags &= ~1u;
}
