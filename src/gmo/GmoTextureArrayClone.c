// bdc 0x08a2577c GmoTextureArrayClone
#include "bdc.h"

/* Build pass for copying `count` `GmoTexture` records: when `GmoTextureNeedsClone` says so, takes
   `count` new records from the plan `arena` (`GmoImagePlanTakeTextures`), copies each with
   `GmoTextureClone` and returns them; otherwise increments each record's reference count and
   returns `images` itself. NULL `images` or `arena` returns NULL. */

void *GmoTextureArrayClone(void *images, s32 count, u32 flags, void *arena)
{
    GmoTexture *src = (GmoTexture *)images;
    GmoTexture *out;
    s32 i;

    if (src == NULL || arena == NULL) {
        return NULL;
    }
    if (GmoTextureNeedsClone(images, count, flags) != 0) {
        out = (GmoTexture *)GmoImagePlanTakeTextures(count, arena);
        for (i = 0; i < count; i++) {
            GmoTextureClone(&out[i], &src[i], flags, arena);
        }
        return out;
    }
    for (i = 0; i < count; i++) {
        src[i].refCount++;
    }
    return images;
}
