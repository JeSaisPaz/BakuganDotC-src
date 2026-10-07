// bdc 0x08970780 UiOptionInitTitle
#include "bdc.h"

/* Shows sprite 0x23 of `UiOption` (title) at full alpha and saves its Y position. */

void UiOptionInitTitle(UiOption *self)
{
    int i = 0x23;
    GfxSprite **sprites = (GfxSprite **)self->base.data;

    do {
        UiOptionShowSprite(&self->base, sprites[i]);
        sprites[i]->alpha = 1.0f;
        self->spriteY[i] = sprites[i]->posY;
        i++;
    } while (i < 0x24);
}
