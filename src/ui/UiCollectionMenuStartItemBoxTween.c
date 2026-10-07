// bdc 0x08974d94 UiCollectionMenuStartItemBoxTween
#include "bdc.h"

/* Starts the item-box model tween of `UiCollectionMenu`: clears the 0x28-byte
   record at `+0x524` (through `spinAngle`), then for `out == 0` fades the model in from alpha 0 with a
   160→40 slide and base angle 0.5; otherwise starts from alpha 1 / scale 1 with a 40→160 slide.
   The slide delta is `|end - start|` (`UiAbsDiff`). */

void UiCollectionMenuStartItemBoxTween(UiCollectionMenu *self, u8 out)
{
    memset(self->itemBoxTween, 0, 0x28);
    if (out == 0) {
        self->itemBox->ambient[3] = 0.0f;
        self->spinScale = 0.0f;
        self->spinT = 0.0f;
        self->spinAngle = 0.5f;
        self->itemBox->pos[0] = 160.0f;
        self->itemBoxSlideEnd = 40;
        self->itemBoxSlideStart = 160;
    } else {
        self->itemBox->ambient[3] = 1.0f;
        self->spinScale = 1.0f;
        self->spinAngle = 0.0f;
        self->spinT = 0.0f;
        self->itemBox->pos[0] = 40.0f;
        self->itemBoxSlideStart = 40;
        self->itemBoxSlideEnd = 160;
    }
    self->itemBoxSlideDelta = (s16)(int)UiAbsDiff((float)self->itemBoxSlideStart,
                                                  (float)self->itemBoxSlideEnd);
}
