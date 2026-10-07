// bdc 0x0896cae8 UiCardEquipFollowCardMarkers
#include "bdc.h"

/* Keeps the card marker sprites (group 16) of `UiCardEquip` at their recorded
   offset below their card icons, scaled by the icon's Y scale. */

void UiCardEquipFollowCardMarkers(UiCardEquip *self)
{
    GfxSprite **sprites = (GfxSprite **)self->base.data;
    s32 i;

    for (i = 0; i < self->groups[8][1]; i++) {
        if (i < self->bakuganCount * 4) {
            GfxSprite *icon = sprites[self->groups[8][0] + i];
            sprites[self->groups[16][0] + i]->posY =
                icon->posY + self->markerOffsetY[i >> 2][i & 3] * icon->scaleY;
        }
    }
}
