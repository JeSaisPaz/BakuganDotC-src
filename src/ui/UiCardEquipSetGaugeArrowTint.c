// bdc 0x0896a2ac UiCardEquipSetGaugeArrowTint
#include "bdc.h"

/* Shows (or hides when `show` = 0) the six gauge arrow triangles of Bakugan `index` in
   `UiCardEquip` (group 14), tinting them with brightness `level` (the first of
   each triple in a blue tint `{0, level/2, level}`), and restores their depths. */

void UiCardEquipSetGaugeArrowTint(float level, UiScreen *screen, u8 show, u8 index)
{
    UiCardEquip *self = (UiCardEquip *)screen;
    float half = level * 0.5f;
    int base = index * 6;
    int i;

    for (i = 0; i < 6; i++) {
        GfxSprite *sprite = ((GfxSprite **)self->base.data)[self->groups[14][0] + base + i];

        if (show == 0) {
            sprite->flags &= ~1u;
            continue;
        }
        UiCardEquipShowSprite(self, sprite);
        sprite = ((GfxSprite **)self->base.data)[self->groups[14][0] + base + i];
        if (i % 3 == 0) {
            sprite->tint[0] = 0.0f;
            sprite->tint[1] = half;
            sprite->tint[2] = level;
        } else {
            sprite->tint[0] = level;
            sprite->tint[1] = level;
            sprite->tint[2] = level;
        }
        sprite->alpha = 1.0f;
        {
            int idx = self->groups[14][0] + base + i;
            ((GfxSprite **)self->base.data)[idx]->posZ = self->spriteDepth[idx];
        }
    }
}
