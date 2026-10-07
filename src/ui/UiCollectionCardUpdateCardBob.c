// bdc 0x08984494 UiCollectionCardUpdateCardBob
#include "bdc.h"

/* While enabled, bobs the owned card art sprites of the current page of
   `UiCollectionCard` up to 8 px ((1 − cos(πt))/2, t += 1/60) around their
   saved Y. */

void UiCollectionCardUpdateCardBob(UiCollectionCard *self)
{
    GfxSprite **sprites;
    int i;

    if (self->bobOn != 0) {
        self->bobT = self->bobT + 0.016666668f;
        sprites = (GfxSprite **)self->base.data;
        for (i = 0; i < 4; i++) {
            if (self->slots[i + self->page * 4] != 0xff) {
                                sprites[13 + i]->posY =
                    self->bobPos[i][1] - (1.0f - __builtin_cosf(self->bobT * 3.1415927f)) * 0.5f * 8.0f;
            }
        }
    }
}
