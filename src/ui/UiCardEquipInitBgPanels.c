// bdc 0x0896b6f8 UiCardEquipInitBgPanels
#include "bdc.h"

/* Initialises the background panel sprites (group 0) of `UiCardEquip`: half-texel
   UV inset, shown at full alpha, every second one mirrored (`GfxSpriteFlipU`). */

void UiCardEquipInitBgPanels(UiCardEquip *self)
{
    GfxSprite **sprites = (GfxSprite **)self->base.data;
    s32 i;

    for (i = self->groups[0][0]; i < self->groups[0][0] + (s8)self->groups[0][1]; i++) {
        GfxSpriteInsetUv(0.5f, sprites[i]);
        UiCardEquipShowSprite(self, sprites[i]);
        sprites[i]->alpha = 1.0f;
        if (((i - self->groups[0][0]) & 1) != 0) {
            GfxSpriteFlipU(sprites[i]);
        }
    }
}
