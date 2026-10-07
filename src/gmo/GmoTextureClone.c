// bdc 0x08a2554c GmoTextureClone
#include "bdc.h"

/* Build pass for copying one texture object: returns NULL when `dst`, `src` or `arena` is NULL
   and `dst` unchanged when `dst == src`. Otherwise drops `flags` to 0 unless
   `GmoTextureNeedsClone` says a copy is needed, releases `dst`'s contents
   (`GmoTextureDestroyContents`), copies the header fields, adds pool-1 references on the shared
   current image / palette (`GmoImageHeapAddRef`), and copies the image list
   (`GmoImageArrayCopy`), palette list (`GmoPaletteArrayCopy`) and `trackCount + 1` tracks
   (`GmoTextureFramesCopy`) with `(flags | 2) & ~1` (0 when no copy). If either list head
   changed it clears the current image and palette (`GmoTextureSetImage` /
   `GmoTextureSetPalette`). Returns `dst`. */

void *GmoTextureClone(void *dstp, const void *srcp, u32 flags, void *arena)
{
    GmoTexture *dst = (GmoTexture *)dstp;
    const GmoTexture *src = (const GmoTexture *)srcp;
    u32 copyFlags;

    if (dst == NULL || src == NULL || arena == NULL) {
        return NULL;
    }
    if (dst == src) {
        return dst;
    }
    if (GmoTextureNeedsClone(src, 1, flags) == 0) {
        flags = 0;
    }
    GmoTextureDestroyContents(dst);
    dst->frameA = src->frameA;
    dst->frameB = src->frameB;
    dst->tracks = src->tracks;
    dst->trackCount = src->trackCount;
    dst->lastTrack = src->lastTrack;
    dst->trackFlags = src->trackFlags;
    dst->frameIndex = src->frameIndex;
    dst->frameC = src->frameC;
    dst->uvTransform[2] = src->uvTransform[2];
    dst->image = src->image;
    dst->flags = src->flags;
    dst->palette = src->palette;
    dst->images = src->images;
    dst->palettes = src->palettes;
    dst->uvTransform[0] = src->uvTransform[0];
    dst->uvTransform[1] = src->uvTransform[1];
    dst->uvTransform[3] = src->uvTransform[3];
    copyFlags = (flags != 0) ? (flags | 2) : 0;
    copyFlags &= ~1u;
    dst->filterMode = src->filterMode;
    dst->wrapU = src->wrapU;
    dst->flagsA = src->flagsA;
    dst->filterMag = src->filterMag;
    dst->wrapV = src->wrapV;
    dst->flagsB = src->flagsB;
    dst->filterMin = src->filterMin;
    GmoImageHeapAddRef(1, dst->image);
    GmoImageHeapAddRef(1, dst->palette);
    dst->images = (GmoImage *)GmoImageArrayCopy(src->images, 1, copyFlags, arena);
    dst->palettes = (GmoImage *)GmoPaletteArrayCopy(src->palettes, 1, copyFlags, arena);
    dst->tracks = GmoTextureFramesCopy(src->tracks, src->trackCount + 1, copyFlags, arena);
    if (dst->images != src->images || dst->palettes != src->palettes) {
        GmoTextureSetImage(dst, NULL);
        GmoTextureSetPalette(dst, NULL);
    }
    return dst;
}
