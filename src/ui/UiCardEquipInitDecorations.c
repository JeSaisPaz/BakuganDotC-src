// bdc 0x0896be28 UiCardEquipInitDecorations
#include "bdc.h"

/* Shows the decoration sprites (group 7) of `UiCardEquip` at alpha 0.8, every
   second one mirrored (`GfxSpriteFlipU`). */

void UiCardEquipInitDecorations(UiCardEquip *self)
{
    GfxSprite **sprites;
    int i;

    for (i = self->groups[7][0]; i < self->groups[7][0] + self->groups[7][1]; i++) {
        sprites = (GfxSprite **)self->base.data;
        UiCardEquipShowSprite(self, sprites[i]);
        ((GfxSprite **)self->base.data)[i]->alpha = 0.8f;
        if (((i - self->groups[7][0]) & 1) != 0) {
            GfxSpriteFlipU(((GfxSprite **)self->base.data)[i]);
        }
    }
}
