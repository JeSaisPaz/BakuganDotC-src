// bdc 0x089abcc8 UiPauseSettingsCreateSprites
#include "bdc.h"

/* Builds the settings screen's sprites (from `UiPauseSettingsPhaseIntro`): clears the 0xa00-byte
   animation state `+0x78..+0xa77` (`animState`), creates the layout-0x39 sprites into the `data`
   sprite table (`UiLayoutCreateSprites`), hides sprites 0..62 (flags bit 0 and alpha cleared)
   and records their `posZ` in `spriteZ`, insets the UVs of the button sprites 0x32..0x38 by half a
   texel (`GfxSpriteInsetUv`), then allocates the pulse ghost sprite 63 from the low heap (NULL if
   the allocation failed), adds it to the layer, copies sprite 0x38 into it and hides it. */

void UiPauseSettingsCreateSprites(UiPauseSettings *self)
{
    bool fromLow;
    GfxSprite *ghost;
    u32 i;

    memset(self->animState, 0, 0xa00);
    UiLayoutCreateSprites(self->base.spriteLayer, (GfxSprite **)self->base.data, 0x39);
    for (i = 0; i < 0x3f; i++) {
        ((GfxSprite **)self->base.data)[i]->flags &= ~1u;
        ((GfxSprite **)self->base.data)[i]->alpha = 0.0f;
        self->spriteZ[i] = ((GfxSprite **)self->base.data)[i]->posZ;
    }
    for (i = 0x32; i < 0x39; i++) {
        GfxSpriteInsetUv(0.5f, ((GfxSprite **)self->base.data)[i]);
    }
    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    ghost = MemAlloc(0x160, NULL, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    if (ghost != NULL) {
        GfxSpriteCtor(ghost);
    }
    ((GfxSprite **)self->base.data)[0x3f] = ghost;
    GfxSpriteLayerAdd(self->base.spriteLayer, (CoreObject *)((GfxSprite **)self->base.data)[0x3f]);
    GfxSpriteCopy(((GfxSprite **)self->base.data)[0x38], ((GfxSprite **)self->base.data)[0x3f]);
    ((GfxSprite **)self->base.data)[0x3f]->flags &= ~1u;
}
