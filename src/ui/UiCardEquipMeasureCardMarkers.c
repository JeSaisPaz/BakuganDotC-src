// bdc 0x0896c1a4 UiCardEquipMeasureCardMarkers
#include "bdc.h"

/* Records, for every card of `UiCardEquip`, the vertical offset of its marker
   sprite (group 16) from its card icon (group 8) into `markerOffsetY[bakugan][card]`, used by
   `UiCardEquipFollowCardMarkers`. */

void UiCardEquipMeasureCardMarkers(UiCardEquip *self)
{
    int i;
    int count = self->groups[8][1];

    for (i = 0; i < count; i++) {
        if (i < self->bakuganCount * 4) {
            GfxSprite **sprites = (GfxSprite **)self->base.data;
            self->markerOffsetY[i / 4][i % 4] =
                sprites[self->groups[0x10][0] + i]->posY - sprites[self->groups[8][0] + i]->posY;
        }
    }
}
