// bdc 0x0898fa74 UiCollectionFigureUpdateZoomIcons
#include "bdc.h"

/* Dims (tint 0.5) the zoom-in icon (data sprite 61) of `UiCollectionFigure`
   when the zoom is at its 1.4 maximum and the zoom-out icon (sprite 62) at the 0.6
   minimum; otherwise shows them at full tint. */

void UiCollectionFigureUpdateZoomIcons(UiCollectionFigure *self)
{
    GfxSprite **sprites = (GfxSprite **)self->base.data;
    GfxSprite *icon = sprites[61];
    float z;

    if (!(self->zoom < 1.4f)) {
        icon->alpha = 1.0f;
        icon->tint[0] = 0.5f;
        icon->tint[1] = 0.5f;
        icon->tint[2] = 0.5f;
        z = self->zoom;
        icon = ((GfxSprite **)self->base.data)[62];
    } else {
        icon->tint[0] = 1.0f;
        icon->tint[1] = 1.0f;
        icon->tint[2] = 1.0f;
        icon->alpha = 1.0f;
        z = self->zoom;
        icon = ((GfxSprite **)self->base.data)[62];
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
