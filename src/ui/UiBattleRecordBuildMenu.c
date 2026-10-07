// bdc 0x08949104 UiBattleRecordBuildMenu
#include "bdc.h"

/* Builds the category menu of the battle-record screen (task 3005, `UiBattleRecordCtor`;
   per-Bakugan win/loss statistics from the save profile, layout package `"data/2d/%s/record.lzs"`):
   allocates the 0x4c-byte sprite block (19 sprite pointers) from low memory into `data` (`+0x1c`),
   binds layout 0x33 to it (`UiLayoutCreateSprites`), prepares the 14 layout sprites for the open
   animation (all but 0 and 11..13 get centred pivots and 1.2x/1.1x scale; all alpha 0), stacks
   sprites 5 and 6 one Z step in front of 1 and 5, clones sprite 6 into slot 18
   (`UiBattleRecordCloneSprite`), sets sprite 12 to cell (0,1), and sets up sprites 16/17 (flags
   bit0 set, alpha 0, 16 shows button icon 2), 15 (bit0 cleared, alpha 0) and 14 (layer mask 2,
   bit0 cleared). */

void UiBattleRecordBuildMenu(UiBattleRecord *self)
{
    bool fromLow;
    GfxSprite **sprites;
    int i;

    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    sprites = MemAlloc(19 * sizeof(GfxSprite *), NULL, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    self->base.data = sprites;
    UiLayoutCreateSprites(self->base.spriteLayer, sprites, 0x33);

    /* the asm re-reads `self->base.data` for every access (sprite stores may alias `self`) */
    for (i = 0; i < 14; i++) {
        if (i != 0 && i != 11 && i != 12 && i != 13) {
            GfxSpriteCenterPivot(((GfxSprite **)self->base.data)[i]);
            GfxSpriteResetMatrix(((GfxSprite **)self->base.data)[i]);
            GfxSpriteSetScaleRotation(((GfxSprite **)self->base.data)[i], 1.2f, 1.1f, 0.0f, false);
        }
        ((GfxSprite **)self->base.data)[i]->alpha = 0.0f;
    }

    sprites = (GfxSprite **)self->base.data;
    sprites[5]->posZ = sprites[1]->posZ - 1.0f;
    sprites = (GfxSprite **)self->base.data;
    sprites[6]->posZ = sprites[5]->posZ - 1.0f;
    ((GfxSprite **)self->base.data)[18] =
        UiBattleRecordCloneSprite(((GfxSprite **)self->base.data)[6], self->base.spriteLayer);
    GfxSpriteSetCell(((GfxSprite **)self->base.data)[12], 0.0f, 1.0f);

    ((GfxSprite **)self->base.data)[16]->flags |= 1;
    ((GfxSprite **)self->base.data)[16]->alpha = 0.0f;
    UiSetButtonIcon(((GfxSprite **)self->base.data)[16], 2);
    ((GfxSprite **)self->base.data)[17]->flags |= 1;
    ((GfxSprite **)self->base.data)[17]->alpha = 0.0f;
    ((GfxSprite **)self->base.data)[15]->flags &= ~1u;
    ((GfxSprite **)self->base.data)[15]->alpha = 0.0f;
    ((GfxSprite **)self->base.data)[14]->layerMask = 2;
    ((GfxSprite **)self->base.data)[14]->flags &= ~1u;
}
