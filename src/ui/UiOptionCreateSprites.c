// bdc 0x089701bc UiOptionCreateSprites
#include "bdc.h"

/* Builds the sprites of `UiOption`: clears the tween records (`slots`, 0x938 bytes),
   creates layout 0x3a (`UiLayoutCreateSprites`, 0x3a sprites into the `data` array), hides them,
   zeroes their alpha and saves their depths (`spriteDepth`) and Y positions (`spriteY`); insets
   sprites 0x30..0x34 by half a texel (`GfxSpriteInsetUv`) and adds a hidden copy of sprite 0x34
   as sprite 0x3a (the button highlight; NULL when the low-heap allocation fails). */

void UiOptionCreateSprites(UiOption *self)
{
  GfxSprite **sprites;
  GfxSprite *sprite;
  bool fromLow;
  u32 i;

  memset(self->slots, 0, 0x938);
  UiLayoutCreateSprites(self->base.spriteLayer, (GfxSprite **)self->base.data, 0x3a);
  sprites = (GfxSprite **)self->base.data;

  for (i = 0; i < 0x3a; i++) {
    sprites[i]->flags &= ~1u;
    sprites[i]->alpha = 0.0f;
    self->spriteDepth[i] = sprites[i]->posZ;
    self->spriteY[i] = sprites[i]->posY;
  }
  for (i = 0x30; i < 0x35; i++) {
    GfxSpriteInsetUv(0.5f, sprites[i]);
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
  sprites[0x3a] = sprite;
  GfxSpriteLayerAdd(self->base.spriteLayer, (CoreObject *)sprites[0x3a]);
  GfxSpriteCopy(sprites[0x34], sprites[0x3a]);
  sprites[0x3a]->flags &= ~1u;
}
