// bdc 0x0896cccc UiCardEquipInitGaugeLimitMarks
#include "bdc.h"

/* Prepares the gauge limit mark sprites (group 12, one per Bakugan) of
   `UiCardEquip`: set up and hidden. */

void UiCardEquipInitGaugeLimitMarks(UiCardEquip *self)
{
    int i;
    int first = self->groups[0xc][0];

    for (i = first; i < self->groups[0xc][0] + self->groups[0xc][1]; i++) {
        if (i - first < self->bakuganCount) {
            UiCardEquipShowSprite(self, ((GfxSprite **)self->base.data)[i]);
            ((GfxSprite **)self->base.data)[i]->alpha = 1.0f;
            ((GfxSprite **)self->base.data)[i]->flags &= ~1u;
            first = self->groups[0xc][0];
        }
    }
}
