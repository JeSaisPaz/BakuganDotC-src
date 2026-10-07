// bdc 0x0894d0d0 UiBattleRecordBuildTotalList
#include "bdc.h"

/* Builds the totals (usage share) table of the battle-record screen (task 3005,
   `UiBattleRecordCtor`): frees the old sprite list (`base.data`) and destroys the old sprite
   layer, creates a new sorted 2D layer and a 0x198-byte sprite list (102 slots) filled from
   layout 0x35, creates the 20 face icons `"baku_face_00"`..`"baku_face_19"` into slots 81..100
   (`UiBattleRecordCreateIconSprite`), clones the list frame (slot 4) into slot 101 as the
   stencil writer (`UiBattleRecordSetListFrame`), makes the row sprites stencil-tested, fills the
   6 visible rows with `UiBattleRecordSetTotalRow`, zeroes the alpha of sprites 6..100, selects
   cell (0, 1) of sprite 2 and sets up the two scroll arrows (sprites 8 and 10: centred, flipped,
   7 px lower). */

void UiBattleRecordBuildTotalList(UiBattleRecord *self)
{
    bool fromLow;
    GfxSpriteLayer *layer;
    GfxSpriteLayer *mem;
    GfxSprite **sprites;
    void *data;
    int row;
    int i;

    data = self->base.data;
    if (data != NULL) {
        MemLock();
        MemFree(data, NULL, 0);
        MemUnlock();
        self->base.data = NULL;
    }
    layer = self->base.spriteLayer;
    self->base.data = NULL;
    if (layer != NULL) {
        const VtblEntry *dtor = &layer->vtbl[1];

        ((void (*)(void *, s32))dtor->fn)((u8 *)layer + dtor->delta, 3);
        self->base.spriteLayer = NULL;
    }

    layer = NULL;
    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    mem = MemAlloc(sizeof(GfxSpriteLayer), NULL, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    if (mem != NULL) {
        GfxSpriteLayerCtor(mem, 0);
        layer = mem;
    }
    self->base.spriteLayer = layer;
    self->base.spriteLayer->sorted = 1;

    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    sprites = MemAlloc(102 * sizeof(GfxSprite *), NULL, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    self->base.data = sprites;
    UiLayoutCreateSprites(self->base.spriteLayer, sprites, 0x35);

    sprites[81] = UiBattleRecordCreateIconSprite("baku_face_00", self->base.spriteLayer);
    sprites[82] = UiBattleRecordCreateIconSprite("baku_face_01", self->base.spriteLayer);
    sprites[83] = UiBattleRecordCreateIconSprite("baku_face_02", self->base.spriteLayer);
    sprites[84] = UiBattleRecordCreateIconSprite("baku_face_03", self->base.spriteLayer);
    sprites[85] = UiBattleRecordCreateIconSprite("baku_face_04", self->base.spriteLayer);
    sprites[86] = UiBattleRecordCreateIconSprite("baku_face_05", self->base.spriteLayer);
    sprites[87] = UiBattleRecordCreateIconSprite("baku_face_06", self->base.spriteLayer);
    sprites[88] = UiBattleRecordCreateIconSprite("baku_face_07", self->base.spriteLayer);
    sprites[89] = UiBattleRecordCreateIconSprite("baku_face_08", self->base.spriteLayer);
    sprites[90] = UiBattleRecordCreateIconSprite("baku_face_09", self->base.spriteLayer);
    sprites[91] = UiBattleRecordCreateIconSprite("baku_face_10", self->base.spriteLayer);
    sprites[92] = UiBattleRecordCreateIconSprite("baku_face_11", self->base.spriteLayer);
    sprites[93] = UiBattleRecordCreateIconSprite("baku_face_12", self->base.spriteLayer);
    sprites[94] = UiBattleRecordCreateIconSprite("baku_face_13", self->base.spriteLayer);
    sprites[95] = UiBattleRecordCreateIconSprite("baku_face_14", self->base.spriteLayer);
    sprites[96] = UiBattleRecordCreateIconSprite("baku_face_15", self->base.spriteLayer);
    sprites[97] = UiBattleRecordCreateIconSprite("baku_face_16", self->base.spriteLayer);
    sprites[98] = UiBattleRecordCreateIconSprite("baku_face_17", self->base.spriteLayer);
    sprites[99] = UiBattleRecordCreateIconSprite("baku_face_18", self->base.spriteLayer);
    sprites[100] = UiBattleRecordCreateIconSprite("baku_face_19", self->base.spriteLayer);

    sprites[101] = UiBattleRecordCloneSprite(sprites[4], self->base.spriteLayer);
    GfxSpriteSetStencilWrite(sprites[101], true, 0);
    UiBattleRecordSetListFrame(sprites[101]);
    sprites[4]->layerMask = 2;
    sprites[4]->flags &= ~1u;

    for (row = 0; row < 6; row++) {
        GfxSpriteSetStencilTest(sprites[13 + row], true, 0);
        GfxSpriteSetStencilTest(sprites[21 + row], true, 0);
        GfxSpriteSetStencilTest(sprites[27 + row], true, 0);
        GfxSpriteSetStencilTest(sprites[33 + row], true, 0);
        GfxSpriteSetStencilTest(sprites[39 + row], true, 0);
        GfxSpriteSetStencilTest(sprites[69 + row], true, 0);
        GfxSpriteSetStencilTest(sprites[75 + row], true, 0);
        for (i = 0; i < 4; i++) {
            GfxSpriteSetStencilTest(sprites[45 + row * 4 + i], true, 0);
        }
        UiBattleRecordSetTotalRow(self, row, row);
    }

    for (i = 6; i < 101; i++) {
        sprites[i]->alpha = 0.0f;
    }

    GfxSpriteSetCell(sprites[2], 0.0f, 1.0f);
    sprites[7]->flags &= ~1u;
    sprites[9]->flags &= ~1u;
    sprites[10]->flags &= ~1u;

    GfxSpriteCenterPivot(sprites[8]);
    GfxSpriteResetMatrix(sprites[8]);
    GfxSpriteFlipV(sprites[8]);
    sprites[8]->posY += 7.0f;

    GfxSpriteCenterPivot(sprites[10]);
    GfxSpriteResetMatrix(sprites[10]);
    GfxSpriteFlipV(sprites[10]);
    sprites[10]->posY += 7.0f;
}
