// bdc 0x08973d78 UiCollectionMenuCreateSprites
#include "bdc.h"

/* Creates the sprites of the collection top menu (task 311, `maybe_UiScreen311Ctor`; entries
   `"Card_light"`, `"Figure_light"`, `"Sphere_light"`, `"Theater_light"`, `"Mark_light"`,
   `"Reset_light"`, a rotating `"menu_itembox.gmo"` model): clears `tweens`, instantiates layout
   0x20 (`UiLayoutCreateSprites`) into the sprite table `base.data`, hides (flags bit0 cleared),
   centres, resets scale/rotation and zero-alphas sprites 0..24 saving their depths in `spriteZ[]`,
   insets the UVs of sprites 18, 19 and 3..8 by half a texel, saves the X of sprites 13..17 in
   `entryX[]`, and allocates sprite 25 as a hidden copy of sprite 12 added to the layer. */

void UiCollectionMenuCreateSprites(UiCollectionMenu *self)
{
  bool fromLow;
  GfxSprite *mem;
  GfxSprite *sprite;
  GfxSprite **sprites;
  u32 i;

  memset(self->tweens, 0, 0x410);
  UiLayoutCreateSprites(self->base.spriteLayer, self->base.data, 0x20);
  for (i = 0; i < 0x19; i++) {
    sprites = (GfxSprite **)self->base.data;
    sprites[i]->flags = sprites[i]->flags & ~1u;
    GfxSpriteCenterPivot(((GfxSprite **)self->base.data)[i]);
    UiSpriteSetScaleRotation(((GfxSprite **)self->base.data)[i], 1.0f, 1.0f, 0.0f);
    ((GfxSprite **)self->base.data)[i]->alpha = 0.0f;
    self->spriteZ[i] = ((GfxSprite **)self->base.data)[i]->posZ;
  }
  GfxSpriteInsetUv(0.5f, ((GfxSprite **)self->base.data)[18]);
  GfxSpriteInsetUv(0.5f, ((GfxSprite **)self->base.data)[19]);
  for (i = 3; i < 9; i++) {
    GfxSpriteInsetUv(0.5f, ((GfxSprite **)self->base.data)[i]);
  }
  sprites = (GfxSprite **)self->base.data;
  for (i = 0; i < 5; i++) {
    self->entryX[i] = sprites[13 + i]->posX;
  }

  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  mem = MemAlloc(sizeof(GfxSprite), NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  sprite = NULL;
  if (mem != NULL) {
    GfxSpriteCtor(mem);
    sprite = mem;
  }
  ((GfxSprite **)self->base.data)[25] = sprite;
  GfxSpriteLayerAdd(self->base.spriteLayer, (CoreObject *)((GfxSprite **)self->base.data)[25]);
  sprites = (GfxSprite **)self->base.data;
  GfxSpriteCopy(sprites[12], sprites[25]);
  sprite = ((GfxSprite **)self->base.data)[25];
  sprite->flags = sprite->flags & ~1u;
}
