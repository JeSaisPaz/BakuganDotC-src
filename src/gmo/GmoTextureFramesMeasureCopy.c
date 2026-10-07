// bdc 0x08a247e4 GmoTextureFramesMeasureCopy
#include "bdc.h"

/* Measure pass for copying an image's frame array: when `flags & 0x43` reserves `count` 0x30-byte
   image records in the plan (`GmoImagePlanReserveImages`). Returns 1, or 0 when `frames` or
   `arena` is NULL. */
s32 GmoTextureFramesMeasureCopy(void *frames, s32 count, u32 flags, void *arena)
{
    if (frames == NULL || arena == NULL) {
        return 0;
    }
    if ((flags & 0x43) != 0) {
        GmoImagePlanReserveImages(count, arena);
    }
    return 1;
}
