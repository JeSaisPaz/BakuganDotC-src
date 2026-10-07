// bdc 0x0891f118 UiHologramGalleryBaseDone
#include "bdc.h"

/* Steps the tweens of the five base sprites (0..4) of the hologram gallery screen with
   `UiSpriteEaseStep` (16 frames, alpha step 0.3, alpha + scale): scale 1.5 → 1.0 when opening
   (`hide == 0`), 1.0 → 1.5 closing when `hide != 0`. Returns true once any of the five steps
   reports finished (u8 count of finished steps != 0). */

bool UiHologramGalleryBaseDone(UiHologramGallery *self, char hide)
{
    u8 finished = 0;
    int i;

    if (hide == 0) {
        for (i = 0; i < 5; i++) {
            finished = (u8)(finished + UiSpriteEaseStep(1.5f, 1.0f, 16.0f, 0.3f, (u8)hide,
                                                        ((GfxSprite **)self->base.data)[i],
                                                        &self->tweens[i], 3));
        }
    } else {
        for (i = 0; i < 5; i++) {
            finished = (u8)(finished + UiSpriteEaseStep(1.0f, 1.5f, 16.0f, 0.3f, (u8)hide,
                                                        ((GfxSprite **)self->base.data)[i],
                                                        &self->tweens[i], 3));
        }
    }
    return finished != 0;
}
