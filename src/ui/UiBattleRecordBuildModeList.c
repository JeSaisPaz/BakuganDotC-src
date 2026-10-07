// bdc 0x0894caa8 UiBattleRecordBuildModeList
#include "bdc.h"

/* Builds the per-mode statistics table of the battle-record screen (task 3005,
   `UiBattleRecordCtor`): frees the old sprite list (`base.data`) and destroys the old sprite
   layer, creates a new sorted 2D layer and a 0x2a0-byte sprite list filled from layout 0x34,
   creates the 20 face icons `"baku_face_00"`..`"baku_face_19"` into slots 147..166
   (`UiBattleRecordCreateIconSprite`), clones the list frame (slot 4) into slot 167 as the stencil
   writer (`UiBattleRecordSetListFrame`), makes the row sprites stencil-tested, fills the 6
   visible rows with `UiBattleRecordSetModeRow`, zeroes the alpha of sprites 6..166, selects the
   mode title cell (sprite 6, row `mode`) and sets up the two scroll arrows (sprites 8 and 10:
   centred, flipped, 7 px lower). */

void UiBattleRecordBuildModeList(UiBattleRecord *self)
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
    mem = MemAlloc(0x80, NULL, 0);
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
    sprites = MemAlloc(0x2a0, NULL, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    self->base.data = sprites;
    UiLayoutCreateSprites(self->base.spriteLayer, sprites, 0x34);

    sprites[147] = UiBattleRecordCreateIconSprite("baku_face_00", self->base.spriteLayer);
    sprites[148] = UiBattleRecordCreateIconSprite("baku_face_01", self->base.spriteLayer);
    sprites[149] = UiBattleRecordCreateIconSprite("baku_face_02", self->base.spriteLayer);
    sprites[150] = UiBattleRecordCreateIconSprite("baku_face_03", self->base.spriteLayer);
    sprites[151] = UiBattleRecordCreateIconSprite("baku_face_04", self->base.spriteLayer);
    sprites[152] = UiBattleRecordCreateIconSprite("baku_face_05", self->base.spriteLayer);
    sprites[153] = UiBattleRecordCreateIconSprite("baku_face_06", self->base.spriteLayer);
    sprites[154] = UiBattleRecordCreateIconSprite("baku_face_07", self->base.spriteLayer);
    sprites[155] = UiBattleRecordCreateIconSprite("baku_face_08", self->base.spriteLayer);
    sprites[156] = UiBattleRecordCreateIconSprite("baku_face_09", self->base.spriteLayer);
    sprites[157] = UiBattleRecordCreateIconSprite("baku_face_10", self->base.spriteLayer);
    sprites[158] = UiBattleRecordCreateIconSprite("baku_face_11", self->base.spriteLayer);
    sprites[159] = UiBattleRecordCreateIconSprite("baku_face_12", self->base.spriteLayer);
    sprites[160] = UiBattleRecordCreateIconSprite("baku_face_13", self->base.spriteLayer);
    sprites[161] = UiBattleRecordCreateIconSprite("baku_face_14", self->base.spriteLayer);
    sprites[162] = UiBattleRecordCreateIconSprite("baku_face_15", self->base.spriteLayer);
    sprites[163] = UiBattleRecordCreateIconSprite("baku_face_16", self->base.spriteLayer);
    sprites[164] = UiBattleRecordCreateIconSprite("baku_face_17", self->base.spriteLayer);
    sprites[165] = UiBattleRecordCreateIconSprite("baku_face_18", self->base.spriteLayer);
    sprites[166] = UiBattleRecordCreateIconSprite("baku_face_19", self->base.spriteLayer);

    sprites[167] = UiBattleRecordCloneSprite(sprites[4], self->base.spriteLayer);
    GfxSpriteSetStencilWrite(sprites[167], true, 0);
    UiBattleRecordSetListFrame(sprites[167]);
    sprites[4]->layerMask = 2;
    sprites[4]->flags &= ~1u;

    for (row = 0; row < 6; row++) {
        GfxSpriteSetStencilTest(sprites[16 + row], true, 0);
        GfxSpriteSetStencilTest(sprites[27 + row], true, 0);
        GfxSpriteSetStencilTest(sprites[33 + row], true, 0);
        GfxSpriteSetStencilTest(sprites[39 + row], true, 0);
        GfxSpriteSetStencilTest(sprites[45 + row], true, 0);
        for (i = 0; i < 16; i++) {
            GfxSpriteSetStencilTest(sprites[51 + row * 16 + i], true, 0);
        }
        UiBattleRecordSetModeRow(self, row, row);
    }

    for (i = 6; i < 167; i++) {
        sprites[i]->alpha = 0.0f;
    }

    GfxSpriteSetCell(sprites[6], 0.0f, (float)self->mode);
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
