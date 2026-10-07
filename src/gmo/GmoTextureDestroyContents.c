// bdc 0x08a127b0 GmoTextureDestroyContents
#include "bdc.h"

/* Releases everything a `GmoTexture` references: image and palette lists, the animation tracks
   (`trackCount` + 1) and the current image/palette references. Returns `tex`. */
void *GmoTextureDestroyContents(void *tex_)
{
    GmoTexture *tex = tex_;

    if (tex != NULL) {
        GmoImageArrayRelease((short *)tex->images, 1);
        GmoImageArrayReleaseThunk((short *)tex->palettes, 1);
        GmoTrackArrayRelease((short *)tex->tracks, tex->trackCount + 1);
        GmoImageHeapReleaseThunk(1, tex->image);
        GmoImageHeapReleaseThunk(1, tex->palette);
    }
    return tex_;
}
