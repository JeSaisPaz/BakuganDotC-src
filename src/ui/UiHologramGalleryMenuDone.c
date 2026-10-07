// bdc 0x0891f7d0 UiHologramGalleryMenuDone
#include "bdc.h"

/* Advances the menu tweens started by `UiHologramGalleryTweenMenu` by one frame with
   `UiTweenUpdate` (scale 1.0 → 1.0, 16 frames, alpha): sprite/tween 5, 6..7 and 12..19.
   Returns true once any of them reports finished (u8 count of finished tweens != 0). */

bool UiHologramGalleryMenuDone(UiHologramGallery *self, u8 hide)
{
    u8 count = 0;
    int i;

    for (i = 5; i < 6; i++) {
        count = (u8)(count + UiTweenUpdate(1.0f, 1.0f, 16.0f, hide,
                                           ((GfxSprite **)self->base.data)[i],
                                           &self->tweens[i], 1));
    }
    for (i = 6; i < 8; i++) {
        count = (u8)(count + UiTweenUpdate(1.0f, 1.0f, 16.0f, hide,
                                           ((GfxSprite **)self->base.data)[i],
                                           &self->tweens[i], 1));
    }
    for (i = 12; i < 20; i++) {
        count = (u8)(count + UiTweenUpdate(1.0f, 1.0f, 16.0f, hide,
                                           ((GfxSprite **)self->base.data)[i],
                                           &self->tweens[i], 1));
    }
    return count != 0;
}
