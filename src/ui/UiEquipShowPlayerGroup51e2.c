// bdc 0x0895ff18 UiEquipShowPlayerGroup51e2
#include "bdc.h"

/* Shows or hides player `player`'s sprite range `+0x51e2 + player*+0x51e4` of the UiEquip
   Bakugan/gear loadout screen (task 302, `UiEquipCtor`). */

void UiEquipShowPlayerGroup51e2(UiEquip *self, u8 player, bool show)
{
    GfxSprite **sprites = (GfxSprite **)self->base.data;
    int i = self->spriteIdx[0x41] + self->spriteIdx[0x42] * player;

    if (show) {
        for (; i < self->spriteIdx[0x41] + self->spriteIdx[0x42] * (player + 1); i++) {
            sprites[i]->flags |= 1;
        }
    } else {
        for (; i < self->spriteIdx[0x41] + self->spriteIdx[0x42] * (player + 1); i++) {
            sprites[i]->flags &= ~1u;
        }
    }
}
