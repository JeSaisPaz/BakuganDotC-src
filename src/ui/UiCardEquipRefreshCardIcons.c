// bdc 0x0896a52c UiCardEquipRefreshCardIcons
#include "bdc.h"

/* Shows the card icon sprites (group 8, four per Bakugan) of `UiCardEquip` that
   belong to existing Bakugan: alpha 1, depth restored from `spriteDepth`, and the texture set from
   `cardIds` (0xff = empty) with `UiCardSetThumbnailTexture` (large layout) or
   `UiGauntletSetupSetCardTexture` (small layout). */

void UiCardEquipRefreshCardIcons(UiCardEquip *self)
{
    int i;
    int idx;
    int small;
    u8 card;
    GfxSprite *sprite;

    for (i = self->groups[8][0]; i < self->groups[8][0] + (s8)self->groups[8][1]; i++) {
        if (i - self->groups[8][0] >= self->bakuganCount * 4)
            continue;
        UiCardEquipShowSprite(self, ((GfxSprite **)self->base.data)[i]);
        ((GfxSprite **)self->base.data)[i]->alpha = 1.0f;
        ((GfxSprite **)self->base.data)[i]->posZ = self->spriteDepth[i];

        idx = i - self->groups[8][0];
        sprite = ((GfxSprite **)self->base.data)[i];
        small = self->bakuganCount < 3;
        card = self->cardIds[(idx / 4) * 4 + idx % 4];
        if (card == 0xff) {
            if (small)
                UiGauntletSetupSetCardTexture(sprite, 0xff);
            else
                UiCardSetThumbnailTexture(sprite, 0xff);
        } else if (small) {
            UiGauntletSetupSetCardTexture(sprite, card);
        } else {
            UiCardSetThumbnailTexture(sprite, card);
        }
    }
}
