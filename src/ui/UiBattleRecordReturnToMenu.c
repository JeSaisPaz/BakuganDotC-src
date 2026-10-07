// bdc 0x0894ac2c UiBattleRecordReturnToMenu
#include "bdc.h"

/* Leaves a statistics list of the battle-record screen (task 3005, `UiBattleRecordCtor`;
   per-Bakugan win/loss statistics from the save profile, layout package `"data/2d/%s/record.lzs"`):
   frees the list's sprite block (`data`) and destroys its layer (vtable slot 1, flags 3), creates
   a new sorted 2D sprite layer from low memory, rebuilds the category menu (layout 0x33 into a
   new 0x4c-byte block, as in `UiBattleRecordBuildMenu` but with the sprites already visible:
   the scaled sprites get alpha 0, sprites 16/17 alpha 1, sprite 15 is only re-centred; sprites
   5 and 6 are moved onto the entry of the current `mode` (sprite 1 + mode)) and sets `phase` 2 /
   `phaseStep` 2 / `animFrame` 0 so `UiBattleRecordMenuPhase` plays the switch animation. */

void UiBattleRecordReturnToMenu(UiBattleRecord *self)
{
    bool fromLow;
    GfxSpriteLayer *layer;
    GfxSpriteLayer *mem;
    GfxSprite **sprites;
    void *data;
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
    layer->sorted = 1;

    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    sprites = MemAlloc(0x4c, NULL, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    self->base.data = sprites;
    UiLayoutCreateSprites(self->base.spriteLayer, sprites, 0x33);

    for (i = 0; i < 14; i++) {
        if (i != 0 && i != 11 && i != 12 && i != 13) {
            GfxSpriteCenterPivot(sprites[i]);
            GfxSpriteResetMatrix(sprites[i]);
            GfxSpriteSetScaleRotation(sprites[i], 1.2f, 1.1f, 0.0f, false);
            sprites[i]->alpha = 0.0f;
        }
    }

    sprites[5]->posZ = sprites[1]->posZ - 1.0f;
    sprites[6]->posZ = sprites[5]->posZ - 1.0f;
    sprites[5]->posX = sprites[1 + self->mode]->posX;
    sprites[5]->posY = sprites[1 + self->mode]->posY;
    sprites[6]->posX = sprites[1 + self->mode]->posX;
    sprites[6]->posY = sprites[1 + self->mode]->posY;
    sprites[18] = UiBattleRecordCloneSprite(sprites[6], self->base.spriteLayer);
    GfxSpriteSetCell(sprites[12], 0.0f, 1.0f);

    sprites[16]->flags |= 1;
    sprites[16]->alpha = 1.0f;
    UiSetButtonIcon(sprites[16], 2);
    sprites[17]->flags |= 1;
    sprites[17]->alpha = 1.0f;
    GfxSpriteCenterPivot(sprites[15]);
    GfxSpriteResetMatrix(sprites[15]);
    sprites[14]->layerMask = 2;
    sprites[14]->flags &= ~1u;

    self->base.phase = 2;
    self->base.phaseStep = 2;
    self->animFrame = 0;
}
