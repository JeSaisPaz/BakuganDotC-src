// bdc 0x08a24958 GmoTextureFramesCopy
#include "bdc.h"

/* Build pass for an image's frame array (`count` 0x30-byte `GmoTexTrack` records): when
   `flags & 0x43` allocates `count` new records from the arena (`GmoImagePlanTakeImages`) and copies
   each (`GmoTextureFrameCopy`), returning the new array; otherwise only bumps each record's
   reference count and returns `frames`. NULL `frames` or `arena` returns NULL. */

void *GmoTextureFramesCopy(void *frames, s32 count, u32 flags, void *arena)
{
    GmoTexTrack *src = (GmoTexTrack *)frames;
    GmoTexTrack *dst;
    s32 i;

    if (src == NULL || arena == NULL) {
        return NULL;
    }
    if ((flags & 0x43) != 0) {
        dst = (GmoTexTrack *)GmoImagePlanTakeImages(count, arena);
        for (i = 0; i < count; i++) {
            GmoTextureFrameCopy(&dst[i], &src[i], flags, arena);
        }
        return dst;
    }
    for (i = 0; i < count; i++) {
        src[i].refCount++;
    }
    return src;
}
