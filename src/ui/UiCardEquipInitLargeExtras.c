// bdc 0x0896d6ec UiCardEquipInitLargeExtras
#include "bdc.h"

/* Large layout only (3+ Bakugan): shows the extra sprites of group 21 of
   `UiCardEquip`, every even one mirrored. */

void UiCardEquipInitLargeExtras(UiCardEquip *self)
{
    int i;

    if (self->bakuganCount > 2) {
        for (i = self->groups[0x15][0]; i < self->groups[0x15][0] + self->groups[0x15][1]; i++) {
            UiCardEquipShowSprite(self, ((GfxSprite **)self->base.data)[i]);
            ((GfxSprite **)self->base.data)[i]->alpha = 1.0f;
            if (((i - self->groups[0x15][0]) & 1) == 0) {
                GfxSpriteFlipU(((GfxSprite **)self->base.data)[i]);
            }
        }
    }
}
