// bdc 0x08a24858 GmoTextureFrameCopy
#include "bdc.h"

/* Copies one 0x30-byte image frame record from `src` to `dst` (init via `GmoTrackDestroyContents`,
   then the timing/texture fields) and takes references on its two texture pointers
   (`GmoImageHeapAddRef(0, …)`). Returns `dst`, or NULL for NULL arguments. */

void *GmoTextureFrameCopy(void *dst, const void *src, u32 flags, void *arena)
{
    GmoTexTrack *d = (GmoTexTrack *)dst;
    const GmoTexTrack *s = (const GmoTexTrack *)src;

    if (d == NULL || s == NULL || arena == NULL) {
        return NULL;
    }
    if (d != s) {
        GmoTrackDestroyContents(d);
        d->startFrame = s->startFrame;
        d->unused02 = s->unused02;
        d->frames = s->frames;
        d->keyCount = s->keyCount;
        d->frameCount = s->frameCount;
        d->mode = s->mode;
        d->endFrame = s->endFrame;
        d->frameRate = s->frameRate;
        d->time = s->time;
        d->blend = s->blend;
        d->frameA0 = s->frameA0;
        d->frameB0 = s->frameB0;
        d->frameA1 = s->frameA1;
        d->frameB1 = s->frameB1;
        d->keys = s->keys;
        GmoImageHeapAddRef(0, s->keys);
        GmoImageHeapAddRef(0, d->frames);
    }
    return d;
}
