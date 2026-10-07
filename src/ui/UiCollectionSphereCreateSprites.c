// bdc 0x08979d80 UiCollectionSphereCreateSprites
#include "bdc.h"

/* Creates the sprites of the sphere (Bakugan figure) collection screen (task 312,
   `maybe_UiScreen312Ctor`; 3x2 grid pages of collected Bakugan shown as 3D models
   (`"00_P_Dragonoid_N_P.gmo"`…), names `"cha_spherename_colle_%02d"`, pop-out motions
   (`"00_dor_dir_popout"`…); cursor `+0xee0`, page `+0xee1`, category `+0xee5`, entry lists
   `+0x1250`): clears the tweens, instantiates layout 0x27 (`UiLayoutCreateSprites`) into the
   sprite table `base.data`, hides/centres/zero-alphas sprites 0..69 saving their positions, insets
   the UVs of sprites 0..6, builds sprite 70 as a hidden copy of sprite 6 and saves the positions of
   the cell sprites 7..13 as `cellPos`. */

void UiCollectionSphereCreateSprites(UiCollectionSphere *self)
{
    GfxSprite **sprites;
    GfxSprite *copy;
    GfxSprite *mem;
    bool fromLow;
    u32 i;

    memset(self->tweens, 0, sizeof(self->tweens));
    UiLayoutCreateSprites(self->base.spriteLayer, (GfxSprite **)self->base.data, 0x27);

    for (i = 0; i < 70; i++) {
        sprites = (GfxSprite **)self->base.data;
        sprites[i]->flags &= ~1u;
        GfxSpriteCenterPivot(((GfxSprite **)self->base.data)[i]);
        UiSpriteSetScaleRotation(((GfxSprite **)self->base.data)[i], 1.0f, 1.0f, 0.0f);
        ((GfxSprite **)self->base.data)[i]->alpha = 0.0f;
        sprites = (GfxSprite **)self->base.data;
        self->spriteZ[i] = sprites[i]->posZ;
        self->spritePos[i][0] = sprites[i]->posX;
        self->spritePos[i][1] = sprites[i]->posY;
    }
    for (i = 0; i < 6; i++) {
        GfxSpriteInsetUv(0.5f, ((GfxSprite **)self->base.data)[i]);
    }
    for (i = 6; i < 7; i++) {
        GfxSpriteInsetUv(0.5f, ((GfxSprite **)self->base.data)[i]);
    }

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
    ((GfxSprite **)self->base.data)[70] = copy;
    GfxSpriteLayerAdd(self->base.spriteLayer, (CoreObject *)((GfxSprite **)self->base.data)[70]);
    sprites = (GfxSprite **)self->base.data;
    GfxSpriteCopy(sprites[6], sprites[70]);
    ((GfxSprite **)self->base.data)[70]->flags &= ~1u;

    sprites = (GfxSprite **)self->base.data + 7;
    for (i = 0; i < 7; i++) {
        self->cellPos[i][0] = sprites[i]->posX;
        self->cellPos[i][1] = sprites[i]->posY;
    }
}
