// bdc 0x08961534 UiEquipHidePlayerRows
#include "bdc.h"

/* Hides player `player`'s equipment-row sprites on the UiEquip Bakugan/gear loadout screen (task
   302, `UiEquipCtor`) at once: for each of three sprite groups (base/stride pairs
   `spriteIdx[0x17]/[0x18]`, `[0x2b]/[0x2c]`, `[0x3b]/[0x3c]`, i.e. `+0x518e/+0x5190`,
   `+0x51b6/+0x51b8`, `+0x51d6/+0x51d8`) it clears the visible bit (bit 0 of `flags`) of sprites
   `base + stride*player` up to `base + stride*(player+1)`. The bounds are re-read every iteration.
    */

void UiEquipHidePlayerRows(UiEquip *self, u8 player)
{
    s32 i;

    for (i = self->spriteIdx[0x17] + self->spriteIdx[0x18] * player;
         i < self->spriteIdx[0x17] + self->spriteIdx[0x18] * (player + 1); i++) {
        ((GfxSprite **)self->base.data)[i]->flags &= ~1u;
    }
    for (i = self->spriteIdx[0x2b] + self->spriteIdx[0x2c] * player;
         i < self->spriteIdx[0x2b] + self->spriteIdx[0x2c] * (player + 1); i++) {
        ((GfxSprite **)self->base.data)[i]->flags &= ~1u;
    }
    for (i = self->spriteIdx[0x3b] + self->spriteIdx[0x3c] * player;
         i < self->spriteIdx[0x3b] + self->spriteIdx[0x3c] * (player + 1); i++) {
        ((GfxSprite **)self->base.data)[i]->flags &= ~1u;
    }
}
