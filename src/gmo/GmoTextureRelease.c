// bdc 0x08a12858 GmoTextureRelease
#include "bdc.h"

/* Drops one reference of a `GmoTexture`; at 0 destroys its contents
   (`GmoTextureDestroyContents`) and frees it. Returns `tex` (even when freed). */
short *GmoTextureRelease(short *tex)
{
    GmoTexture *t = (GmoTexture *)tex;

    if (t != NULL) {
        t->refCount = t->refCount - 1;
        if (t->refCount == 0) {
            GmoTextureDestroyContents(t);
            GmoImageHeapReleaseThunk(0, t);
            return tex;
        }
    }
    return tex;
}
