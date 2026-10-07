// bdc 0x089edc38 GfxFaderStep
#include "bdc.h"

/* Advances a fader one frame: while `elapsed < duration` increments `elapsed`, sets `progress =
   elapsed/duration` and lerps the current colour from start to end (`start + (end - start) *
   progress`); afterwards sets `progress = 1` and holds the end colour. Then calls the fader's
   apply method (vtable `+0x24`). */

void GfxFaderStep(GfxFader *self)
{
    const VtblEntry *entry;
    int i;

    if (self->elapsed < self->duration) {
        float progress;

        self->elapsed++;
        progress = (float)self->elapsed / (float)self->duration;
        self->progress = progress;
        for (i = 0; i < 4; i++) {
            self->color[i] = self->start[i] + (self->end[i] - self->start[i]) * progress;
        }
    } else {
        self->progress = 1.0f;
        for (i = 0; i < 4; i++) {
            self->color[i] = self->end[i];
        }
    }
    entry = &self->vtbl[4];
    ((void (*)(void *))entry->fn)((char *)self + entry->delta);
}
