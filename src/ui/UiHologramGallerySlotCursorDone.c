// bdc 0x08920fc8 UiHologramGallerySlotCursorDone
#include "bdc.h"

/* Advances the tweens of sprites 0xb4..0xb5, 0xb7..0xba, 0x80 and 0x82 of the hologram gallery
   screen (`UiHologramGalleryCtor`, task 391) with `UiTweenUpdate` (scale 1 -> 1, 16 frames,
   flags 1) and returns true once at least one of them has finished. */

bool UiHologramGallerySlotCursorDone(UiHologramGallery *self, u8 hide)
{
    GfxSprite **sprites;
    u8 finished = 0;
    int i;

    for (i = 0xb4; i < 0xb6; i++) {
        sprites = (GfxSprite **)self->base.data;
        finished += UiTweenUpdate(1.0f, 1.0f, 16.0f, hide, sprites[i], &self->tweens[i], 1);
    }
    for (i = 0xb7; i < 0xbb; i++) {
        sprites = (GfxSprite **)self->base.data;
        finished += UiTweenUpdate(1.0f, 1.0f, 16.0f, hide, sprites[i], &self->tweens[i], 1);
    }
    for (i = 0x80; i < 0x81; i++) {
        sprites = (GfxSprite **)self->base.data;
        finished += UiTweenUpdate(1.0f, 1.0f, 16.0f, hide, sprites[i], &self->tweens[i], 1);
    }
    for (i = 0x82; i < 0x83; i++) {
        sprites = (GfxSprite **)self->base.data;
        finished += UiTweenUpdate(1.0f, 1.0f, 16.0f, hide, sprites[i], &self->tweens[i], 1);
    }
    return finished != 0;
}
