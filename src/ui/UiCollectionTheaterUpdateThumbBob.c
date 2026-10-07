// bdc 0x089894f8 UiCollectionTheaterUpdateThumbBob
#include "bdc.h"

/* While enabled, bobs the unlocked thumbnails of the current page of
   `UiCollectionTheater` up to 4 px ((1 − cos(πt))/2, t += 1/30) around
   their saved Y. */

void UiCollectionTheaterUpdateThumbBob(UiScreen *screen)
{
    UiCollectionTheater *self = (UiCollectionTheater *)screen;
    GfxSprite **sprites;
    float c;
    int i;

    if (self->thumbBobOn != 0) {
        self->thumbBobT = self->thumbBobT + 0.033333335f;
        sprites = (GfxSprite **)self->base.data;
        for (i = 0; i < 6; i++) {
            if (self->sceneId[i + self->page * 6] != 0xff) {
                /* vcos.s of (t·π)·S703 (2/π): cos in radians */
                c = __builtin_cosf(self->thumbBobT * 3.1415927f);
                sprites[13 + i]->posY =
                    self->thumbPos[i][1] - (1.0f - c) * 0.5f * 4.0f;
            }
        }
    }
}
