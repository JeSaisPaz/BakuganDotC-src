// bdc 0x08921f6c UiHologramGalleryCountRefund
#include "bdc.h"

/* One frame of the pending-amount count of the hologram gallery screen (`UiHologramGalleryCtor`,
   task 391): takes 100 off `bonusAmount` and adds it to (`refund == 0`) or subtracts it from
   (`refund != 0`) profile word 45, then redraws word 45 (sprite 43, colour 1) and the profile
   `points` minus word 45 (sprite 50) with `UiHologramGallerySetNumber`. Returns 1 when no amount
   was pending, else 0. */

int UiHologramGalleryCountRefund(UiHologramGallery *self, char refund)
{
    GfxSprite *sprite;
    u32 word;
    s32 points;

    if ((u8)refund == 0) {
        if (self->bonusAmount == 0) {
            return 1;
        }
        self->bonusAmount = self->bonusAmount - 100;
        word = SaveProfileGetWord(SaveGetProfile(), 0x2d);
        SaveProfileSetWord(SaveGetProfile(), 0x2d, word + 100);
    } else {
        if (self->bonusAmount == 0) {
            return 1;
        }
        self->bonusAmount = self->bonusAmount - 100;
        word = SaveProfileGetWord(SaveGetProfile(), 0x2d);
        SaveProfileSetWord(SaveGetProfile(), 0x2d, word - 100);
    }
    word = SaveProfileGetWord(SaveGetProfile(), 0x2d);
    sprite = ((GfxSprite **)self->base.data)[43];
    UiHologramGallerySetNumber(self, (s32)word, 0x2b, 1, sprite->posX, sprite->posY);
    points = SaveGetProfile()->data->points;
    word = SaveProfileGetWord(SaveGetProfile(), 0x2d);
    sprite = ((GfxSprite **)self->base.data)[50];
    UiHologramGallerySetNumber(self, points - (s32)word, 0x32, 0, sprite->posX, sprite->posY);
    return 0;
}
