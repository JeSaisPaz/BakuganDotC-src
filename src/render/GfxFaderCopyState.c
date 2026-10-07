// bdc 0x089edd50 GfxFaderCopyState
#include "bdc.h"

/* Copies a fader's state (flags, timing, sort key and the current/start/end colours) from `src` to
   `dst`; used by `GfxFaderSelectSlot` to carry the fade over to another slot. */

void GfxFaderCopyState(GfxFader *dst, const GfxFader *src)
{
    int i;

    dst->active = src->active;
    dst->done = src->done;
    dst->duration = src->duration;
    dst->elapsed = src->elapsed;
    dst->progress = src->progress;
    dst->sortKey = src->sortKey;
    for (i = 0; i < 4; i++) {
        dst->color[i] = src->color[i];
    }
    for (i = 0; i < 4; i++) {
        dst->start[i] = src->start[i];
    }
    for (i = 0; i < 4; i++) {
        dst->end[i] = src->end[i];
    }
}
