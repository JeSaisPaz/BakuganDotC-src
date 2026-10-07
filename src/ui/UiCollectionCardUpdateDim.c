// bdc 0x08984df0 UiCollectionCardUpdateDim
#include "bdc.h"

/* Advances dim record `index` of `UiCollectionCard` by 1/16 per frame
   (ease-out to 0.7, or ease-in to 0). Returns 1 when finished. */

u8 UiCollectionCardUpdateDim(UiCollectionCard *self, u8 out, u8 index)
{
    u8 done = 0;
    float t = self->dim[index].t + 0.0625f;
    float base = self->dim[index].base;

    if (out == 0) {
        self->dim[index].t = t;
        self->dim[index].value = base + (1.0f - (t - 1.0f) * (t - 1.0f)) * 0.7f;
        if (!(t < 1.0f)) {
            self->dim[index].value = 0.7f;
            return 1;
        }
    } else {
        self->dim[index].t = t;
        self->dim[index].value = base - t * t * 0.7f;
        if (!(t < 1.0f)) {
            done = 1;
            self->dim[index].value = 0.0f;
        }
    }
    return done;
}
