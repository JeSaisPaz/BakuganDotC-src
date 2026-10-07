// bdc 0x0898eec0 UiCollectionFigureDimFadeDone
#include "bdc.h"

/* Advances background-dimming fade `layer` of `UiCollectionFigure` by 1/16 per frame
   (ease-out to 0.7, or ease-in to 0). Returns 1 when finished. */

u8 UiCollectionFigureDimFadeDone(UiCollectionFigure *self, u8 out, u8 layer)
{
    u8 done = 0;
    float t = self->dim[layer].t + 0.0625f;
    float base = self->dim[layer].base;

    if (out == 0) {
        self->dim[layer].t = t;
        self->dim[layer].value = base + (1.0f - (t - 1.0f) * (t - 1.0f)) * 0.7f;
        if (!(t < 1.0f)) {
            self->dim[layer].value = 0.7f;
            return 1;
        }
    } else {
        self->dim[layer].t = t;
        self->dim[layer].value = base - t * t * 0.7f;
        if (!(t < 1.0f)) {
            done = 1;
            self->dim[layer].value = 0.0f;
        }
    }
    return done;
}
