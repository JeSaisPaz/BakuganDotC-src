// bdc 0x08985edc UiCollectionCardDimZoomIcons
#include "bdc.h"

/* Greys out the zoom-in icon (sprite 0x3a) of `UiCollectionCard` at the
   maximum card scale 1.5 and the zoom-out icon (0x3b) at the minimum 0.6. */

void UiCollectionCardDimZoomIcons(UiCollectionCard *self)
{
    GfxSprite **sprites = (GfxSprite **)self->base.data;
    GfxSprite *icon = sprites[58];

    if (sprites[13 + self->cursor]->scaleX < 1.5f) {
        icon->tint[0] = 1.0f;
        icon->tint[1] = 1.0f;
        icon->tint[2] = 1.0f;
        icon->alpha = 1.0f;
    } else {
        icon->alpha = 1.0f;
        icon->tint[0] = 0.5f;
        icon->tint[1] = 0.5f;
        icon->tint[2] = 0.5f;
    }
    icon = ((GfxSprite **)self->base.data)[59];
    if (((GfxSprite **)self->base.data)[13 + self->cursor]->scaleX <= 0.6f) {
        icon->alpha = 1.0f;
        icon->tint[0] = 0.5f;
        icon->tint[1] = 0.5f;
        icon->tint[2] = 0.5f;
    } else {
        icon->tint[0] = 1.0f;
        icon->tint[1] = 1.0f;
        icon->tint[2] = 1.0f;
        icon->alpha = 1.0f;
    }
}
