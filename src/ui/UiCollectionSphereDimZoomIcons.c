// bdc 0x0897e8f0 UiCollectionSphereDimZoomIcons
#include "bdc.h"

/* Greys out the zoom-in icon (sprite 0x3f) of `UiCollectionSphere` at the
   maximum zoom 1.4 and the zoom-out icon (0x40) at the minimum 0.6. */

void UiCollectionSphereDimZoomIcons(UiCollectionSphere *self)
{
    GfxSprite **sprites = (GfxSprite **)self->base.data;
    GfxSprite *icon = sprites[63];
    float z;

    if (!(self->zoom < 1.4f)) {
        icon->alpha = 1.0f;
        icon->tint[0] = 0.5f;
        icon->tint[1] = 0.5f;
        icon->tint[2] = 0.5f;
        z = self->zoom;
        icon = ((GfxSprite **)self->base.data)[64];
    } else {
        icon->tint[0] = 1.0f;
        icon->tint[1] = 1.0f;
        icon->tint[2] = 1.0f;
        icon->alpha = 1.0f;
        z = self->zoom;
        icon = ((GfxSprite **)self->base.data)[64];
    }
    if (!(z <= 0.6f)) {
        icon->tint[0] = 1.0f;
        icon->tint[1] = 1.0f;
        icon->tint[2] = 1.0f;
        icon->alpha = 1.0f;
        return;
    }
    icon->alpha = 1.0f;
    icon->tint[0] = 0.5f;
    icon->tint[1] = 0.5f;
    icon->tint[2] = 0.5f;
}
