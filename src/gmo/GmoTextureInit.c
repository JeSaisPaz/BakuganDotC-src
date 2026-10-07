// bdc 0x08a12498 GmoTextureInit
#include "bdc.h"

/* Initialises a `GmoTexture`: refcount 1, no images/palettes/tracks, frame selectors `0xff` for
   `frameB`, default scale (`g_gmoTextureDefaultScale`) in the uvTransform scale slots, filter
   defaults. Returns the record (NULL stays NULL). */
void *GmoTextureInit(void *tex_)
{
    GmoTexture *tex = tex_;
    float scale;

    scale = g_gmoTextureDefaultScale;
    if (tex != NULL) {
        tex->frameB = 0xff;
        tex->uvTransform[3] = scale;
        tex->wrapU = 1;
        tex->wrapV = 7;
        tex->refCount = 1;
        tex->flags = 0;
        tex->image = NULL;
        tex->palette = NULL;
        tex->images = NULL;
        tex->palettes = NULL;
        tex->tracks = NULL;
        tex->trackCount = 0;
        tex->lastTrack = 0xff;
        tex->trackFlags = 0;
        tex->frameIndex = 0;
        tex->frameA = 0;
        tex->frameC = 0;
        tex->uvTransform[0] = 0;
        tex->uvTransform[1] = 0;
        tex->uvTransform[2] = scale;
        tex->filterMin = 1;
        tex->filterMode = 0;
        tex->filterMag = 1;
        tex->flagsA = 0;
        tex->flagsB = 0;
    }
    return tex;
}
