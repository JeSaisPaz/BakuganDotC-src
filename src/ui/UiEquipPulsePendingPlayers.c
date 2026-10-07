// bdc 0x0895c40c UiEquipPulsePendingPlayers
#include "bdc.h"

/* While `pulseOn` is set, advances `pulseT` by 1/15 and sets `pulseAlpha` = 1 - (1 - cos(pi*t))/2,
   then updates the two player label sprite ranges (`spriteIdx[8]..`, `spriteIdx[10]..`, one sprite
   per player) of the Bakugan/gear loadout screen before a battle (task 302, `UiEquipCtor`).
   Offline (`SaveGetProfileFlag0` == 0): the first range is hidden (flag bit 0 cleared), the
   players after the one being edited also get the pulsing alpha; in the second range the players
   already done are shown (bit 0 set) with the pulsing alpha, the others hidden. Otherwise both ranges
   only get the pulsing alpha. */

void UiEquipPulsePendingPlayers(UiEquip *self)
{
    GfxSprite *sprite;
    float c;
    int i;
    int k;

    if (self->pulseOn != 0) {
        self->pulseT = self->pulseT + 0.06666667f;
        c = __builtin_cosf(self->pulseT * 3.1415927f);
        self->pulseAlpha = 1.0f - (1.0f - c) * 0.5f;
        if (SaveGetProfileFlag0() == 0) {
            for (i = self->spriteIdx[8]; i < self->spriteIdx[8] + self->playerCount; i++) {
                k = i - self->spriteIdx[8];
                if (k < self->playerCount) {
                    sprite = ((GfxSprite **)self->base.data)[i];
                    if (self->editPlayer < k) {
                        sprite->flags &= ~1u;
                        ((GfxSprite **)self->base.data)[i]->alpha = self->pulseAlpha;
                    } else {
                        sprite->flags &= ~1u;
                    }
                }
            }
            for (i = self->spriteIdx[10]; i < self->spriteIdx[10] + self->playerCount; i++) {
                k = i - self->spriteIdx[10];
                if (k < self->playerCount) {
                    sprite = ((GfxSprite **)self->base.data)[i];
                    if (k < self->doneCount) {
                        sprite->flags |= 1u;
                        ((GfxSprite **)self->base.data)[i]->alpha = self->pulseAlpha;
                    } else {
                        sprite->flags &= ~1u;
                    }
                }
            }
        } else {
            for (i = self->spriteIdx[8]; i < self->spriteIdx[8] + self->playerCount; i++) {
                ((GfxSprite **)self->base.data)[i]->alpha = self->pulseAlpha;
            }
            for (i = self->spriteIdx[10]; i < self->spriteIdx[10] + self->playerCount; i++) {
                ((GfxSprite **)self->base.data)[i]->alpha = self->pulseAlpha;
            }
        }
    }
}
