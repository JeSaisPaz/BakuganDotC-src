// bdc 0x08922430 UiHologramGalleryCountBonus
#include "bdc.h"

/* One frame of the bonus count-up of the hologram gallery screen (`UiHologramGalleryCtor`, task
   391): moves 10 of the pending `bonusAmount` into the profile `points` (clamped to 0..9999999)
   and redraws the point total (sprite 36) and the points minus profile word 45 (sprite 50) with
   `UiHologramGallerySetNumber`. Returns 1 when no bonus was pending or the points reached the
   cap, else 0. */

int UiHologramGalleryCountBonus(UiHologramGallery *self)
{
    SaveProfile *profile;
    GfxSprite *sprite;
    s32 points;
    s32 clamped;
    s32 total;
    u32 word;

    if (self->bonusAmount == 0) {
        return 1;
    }
    self->bonusAmount = self->bonusAmount - 10;
    points = SaveGetProfile()->data->points + 10;
    profile = SaveGetProfile();
    if (9999999 < points) {
        clamped = 9999999;
    } else {
        clamped = points;
        if (clamped < 0) {
            clamped = 0;
        }
    }
    profile->data->points = clamped;
    profile = SaveGetProfile();
    sprite = ((GfxSprite **)self->base.data)[36];
    UiHologramGallerySetNumber(self, profile->data->points, 0x24, 0, sprite->posX, sprite->posY);
    total = SaveGetProfile()->data->points;
    word = SaveProfileGetWord(SaveGetProfile(), 0x2d);
    sprite = ((GfxSprite **)self->base.data)[50];
    UiHologramGallerySetNumber(self, total - (s32)word, 0x32, 0, sprite->posX, sprite->posY);
    if (points < 9999999) {
        return 0;
    }
    return 1;
}
