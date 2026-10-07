// bdc 0x089583b4 UiEquipCreateSprites
#include "bdc.h"

/* Creates the sprites of the Bakugan/gear loadout screen before a battle (task 302,
   `UiEquipCtor`): clears the tween block `tweens`, instantiates layout 0x2c (up to two
   players, 0xec sprites) or 0x2d (four players, 0x177 sprites) into the sprite table
   `base.data` with `UiLayoutCreateSprites`, stores the count in `spriteCount`, insets the UVs
   of sprite 43 and of sprite 132 (two players) / 172 (four players) by half a texel, hides every
   layout sprite (flags bit 0 cleared, alpha 0) while recording its Z in `spriteZ`, then appends
   one extra sprite at index `spriteCount`: allocated from the low heap, constructed
   (`GfxSpriteCtor`; NULL stays NULL), added to the layer, given a copy of sprite 43
   (`GfxSpriteCopy`) and hidden. */

void UiEquipCreateSprites(UiEquip *self)
{
  bool fromLow;
  GfxSprite *sprite;
  GfxSprite *extra;
  GfxSprite **sprites;
  u32 count;
  u32 i;

  memset(self->tweens, 0, 0x3ac0);
  if (self->playerCount < 3) {
    UiLayoutCreateSprites(self->base.spriteLayer, (GfxSprite **)self->base.data, 0x2c);
    self->spriteCount = 0xec;
    count = 0xec;
    GfxSpriteInsetUv(0.5f, ((GfxSprite **)self->base.data)[43]);
    GfxSpriteInsetUv(0.5f, ((GfxSprite **)self->base.data)[132]);
  }
  else {
    UiLayoutCreateSprites(self->base.spriteLayer, (GfxSprite **)self->base.data, 0x2d);
    self->spriteCount = 0x177;
    count = 0x177;
    GfxSpriteInsetUv(0.5f, ((GfxSprite **)self->base.data)[43]);
    GfxSpriteInsetUv(0.5f, ((GfxSprite **)self->base.data)[172]);
  }
  for (i = 0; i < count; i++) {
    sprites = (GfxSprite **)self->base.data;
    sprites[i]->flags &= ~1u;
    sprites[i]->alpha = 0.0f;
    self->spriteZ[i] = sprites[i]->posZ;
  }
  self->animFlags = 0;

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
  ((GfxSprite **)self->base.data)[self->spriteCount] = extra;
  GfxSpriteLayerAdd(self->base.spriteLayer,
                    (CoreObject *)((GfxSprite **)self->base.data)[self->spriteCount]);
  sprites = (GfxSprite **)self->base.data;
  GfxSpriteCopy(sprites[43], sprites[self->spriteCount]);
  sprites = (GfxSprite **)self->base.data;
  sprites[self->spriteCount]->flags &= ~1u;
}
