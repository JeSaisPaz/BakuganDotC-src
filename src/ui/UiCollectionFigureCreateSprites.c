// bdc 0x0898bdf0 UiCollectionFigureCreateSprites
#include "bdc.h"

/* Creates the sprites of the figure collection screen (task 314, `maybe_UiScreen314Ctor`):
   clears the tween block (`tweens`, 0xac8 bytes), instantiates layout 0x29 into the sprite
   table `base.data` (`UiLayoutCreateSprites`), then for sprites 0..67 clears flags bit0,
   centres the pivot, sets scale 1 x 1 / angle 0 and alpha 0, and saves their position in
   `spriteZ` / `spritePos`. Sprites 0..6 get their UVs inset by half a texel. Sprite 68 is a
   new sprite (allocated from the low heap end), added to the layer as a copy of sprite 6 with
   flags bit0 cleared. Finally the six cell anchors are the positions of sprites 7..12, y + 24. */

void UiCollectionFigureCreateSprites(UiCollectionFigure *self)
{
    bool fromLow;
    GfxSprite *sprite;
    GfxSprite *copy;
    u32 i;

    memset(self->tweens, 0, 0xac8);
    UiLayoutCreateSprites(self->base.spriteLayer, self->base.data, 0x29);
    for (i = 0; i < 68; i++) {
        ((GfxSprite **)self->base.data)[i]->flags &= ~1u;
        GfxSpriteCenterPivot(((GfxSprite **)self->base.data)[i]);
        UiSpriteSetScaleRotation(((GfxSprite **)self->base.data)[i], 1.0f, 1.0f, 0.0f);
        ((GfxSprite **)self->base.data)[i]->alpha = 0.0f;
        sprite = ((GfxSprite **)self->base.data)[i];
        self->spriteZ[i] = sprite->posZ;
        self->spritePos[i][0] = sprite->posX;
        self->spritePos[i][1] = sprite->posY;
    }
    for (i = 0; i < 6; i++) {
        GfxSpriteInsetUv(0.5f, ((GfxSprite **)self->base.data)[i]);
    }
    for (i = 6; i < 7; i++) {
        GfxSpriteInsetUv(0.5f, ((GfxSprite **)self->base.data)[i]);
    }

    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    sprite = MemAlloc(sizeof(GfxSprite), NULL, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    copy = NULL;
    if (sprite != NULL) {
        GfxSpriteCtor(sprite);
        copy = sprite;
    }
    ((GfxSprite **)self->base.data)[68] = copy;
    GfxSpriteLayerAdd(self->base.spriteLayer, (CoreObject *)((GfxSprite **)self->base.data)[68]);
    GfxSpriteCopy(((GfxSprite **)self->base.data)[6], ((GfxSprite **)self->base.data)[68]);
    ((GfxSprite **)self->base.data)[68]->flags &= ~1u;

    for (i = 0; i < 6; i++) {
        self->cellAnchor[i][0] = ((GfxSprite **)self->base.data)[7 + i]->posX;
        self->cellAnchor[i][1] = ((GfxSprite **)self->base.data)[7 + i]->posY + 24.0f;
    }
}
