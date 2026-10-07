// bdc 0x08961158 UiEquipStartCommitTween
#include "bdc.h"

/* Starts the close tween (`UiEquipStartSpriteFadeTween(self, 0, i)`) of player `player`'s
   equipment-row sprites on the Bakugan/gear loadout screen before a battle (task 302,
   `UiEquipCtor`): for the three base/stride groups `spriteIdx[0x17]/[0x18]`,
   `[0x2b]/[0x2c]` and `[0x3b]/[0x3c]` (`+0x518e/+0x5190`, `+0x51b6/+0x51b8`,
   `+0x51d6/+0x51d8`) it tweens sprites `base + stride*player` up to `base + stride*(player+1)`.
   In the third group each sprite also gets the thumbnail of the matching `gearPick[player]`
   card (`UiCardSetThumbnailTexture`), or is hidden (bit 0 of `flags` cleared) when that entry
   is 0xff. The bounds are re-read every iteration. */

void UiEquipStartCommitTween(UiEquip *self, u8 player)
{
    s32 i;

    for (i = self->spriteIdx[0x17] + self->spriteIdx[0x18] * player;
         i < self->spriteIdx[0x17] + self->spriteIdx[0x18] * (player + 1); i++) {
        UiEquipStartSpriteFadeTween(self, 0, (u16)i);
    }
    for (i = self->spriteIdx[0x2b] + self->spriteIdx[0x2c] * player;
         i < self->spriteIdx[0x2b] + self->spriteIdx[0x2c] * (player + 1); i++) {
        UiEquipStartSpriteFadeTween(self, 0, (u16)i);
    }
    for (i = self->spriteIdx[0x3b] + self->spriteIdx[0x3c] * player;
         i < self->spriteIdx[0x3b] + self->spriteIdx[0x3c] * (player + 1); i++) {
        GfxSprite *sprite;
        u8 card;

        UiEquipStartSpriteFadeTween(self, 0, (u16)i);
        sprite = ((GfxSprite **)self->base.data)[i];
        card = self->gearPick[player][i - (self->spriteIdx[0x3b] + self->spriteIdx[0x3c] * player)];
        if (card == 0xff) {
            sprite->flags &= ~1u;
        } else {
            UiCardSetThumbnailTexture(sprite, card);
        }
    }
}
