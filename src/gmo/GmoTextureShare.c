// bdc 0x08a258e4 GmoTextureShare
#include "bdc.h"

/* Returns a reference to image `img` for a new owner: when no copy is needed
   (`GmoTextureNeedsClone`) it just increments the reference count (`+0`) and returns `img`;
   otherwise it deep-copies it in two passes with the two-pass arena object (a 0x70-byte stack
   object: `GmoImagePlanInit` init, the measure pass reserves sizes, `GmoImagePlanCommit` allocates,
   `GmoImagePlanFree` finishes) (`GmoTextureArrayMeasureClone`, then `GmoTextureArrayClone`) and
   returns the copy (NULL on failure). */

void *GmoTextureShare(void *img, u32 flags)
{
    GmoTexture *tex = (GmoTexture *)img;
    u8 plan[0x70];

    if (tex == NULL) {
        return NULL;
    }
    if (GmoTextureNeedsClone(tex, 1, flags) == 0) {
        tex->refCount++;
        return tex;
    }
    GmoImagePlanInit(plan);
    if (GmoTextureArrayMeasureClone(tex, 1, flags, plan) == 0) {
        return NULL;
    }
    if (GmoImagePlanCommit(plan) == 0) {
        return NULL;
    }
    tex = GmoTextureArrayClone(tex, 1, flags, plan);
    GmoImagePlanFree(plan);
    return tex;
}
