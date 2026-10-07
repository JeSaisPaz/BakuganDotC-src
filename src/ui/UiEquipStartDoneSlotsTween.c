// bdc 0x08961724 UiEquipStartDoneSlotsTween
#include "bdc.h"

/* Starts (`out` = 0/1) the tweens of the finished-player rows of the Bakugan/gear loadout screen
   before a battle (task 302, `UiEquipCtor`) through `UiEquipStartDoneSlotSpriteTween`: for each
   of the `doneCount` players already done, the sprite ranges `spriteIdx[0x17] + spriteIdx[0x18]*p`,
   `spriteIdx[0x2b] + spriteIdx[0x2c]*p` and `spriteIdx[0x3b] + spriteIdx[0x3c]*p`; the last range
   shows the player's chosen gear (`gearPick[p]`) as card thumbnails (`UiCardSetThumbnailTexture`),
   hiding (flag bit 0 cleared) the slots that hold no card (0xff). */

void UiEquipStartDoneSlotsTween(UiEquip *self, u8 out)
{
    GfxSprite *sprite;
    int player;
    int i;
    u8 p;
    u8 card;

    for (player = 0; player < self->doneCount; player++) {
        p = (u8)player;
        for (i = self->spriteIdx[0x17] + self->spriteIdx[0x18] * p;
             i < self->spriteIdx[0x17] + self->spriteIdx[0x18] * (p + 1); i++) {
            UiEquipStartDoneSlotSpriteTween(self, out, (u16)i);
        }
        for (i = self->spriteIdx[0x2b] + self->spriteIdx[0x2c] * p;
             i < self->spriteIdx[0x2b] + self->spriteIdx[0x2c] * (p + 1); i++) {
            UiEquipStartDoneSlotSpriteTween(self, out, (u16)i);
        }
        for (i = self->spriteIdx[0x3b] + self->spriteIdx[0x3c] * p;
             i < self->spriteIdx[0x3b] + self->spriteIdx[0x3c] * (p + 1); i++) {
            UiEquipStartDoneSlotSpriteTween(self, out, (u16)i);
            card = self->gearPick[p][i - (self->spriteIdx[0x3b] + self->spriteIdx[0x3c] * p)];
            sprite = ((GfxSprite **)self->base.data)[i];
            if (card == 0xff) {
                sprite->flags &= ~1u;
            } else {
                UiCardSetThumbnailTexture(sprite, card);
            }
        }
    }
}
