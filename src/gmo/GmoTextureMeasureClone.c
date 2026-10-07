// bdc 0x08a24d94 GmoTextureMeasureClone
#include "bdc.h"

/* Measure pass for deep-copying one texture object: if `GmoTextureNeedsClone` says so, measures
   its pixel images (`GmoImageArrayMeasureCopy` on `images`), palettes (`GmoPaletteArrayMeasureCopy`
   on `palettes`) and frame tracks (`GmoTextureFramesMeasureCopy` on `tracks`, `trackCount + 1`
   records) with `flags | 2` minus bit 0 (0 stays 0). Returns 0 for a NULL `src` or `arena`, else 1. */

s32 GmoTextureMeasureClone(void *dst, const void *src, u32 flags, void *arena)
{
    const GmoTexture *tex = (const GmoTexture *)src;
    u32 f;

    if (src == NULL || arena == NULL) {
        return 0;
    }
    if (src != dst) {
        s32 need = GmoTextureNeedsClone(src, 1, flags);
        f = 0;
        if (flags != 0) {
            f = flags | 2;
        }
        f &= ~1u;
        if (need != 0) {
            GmoImageArrayMeasureCopy(tex->images, 1, f, arena);
            GmoPaletteArrayMeasureCopy(tex->palettes, 1, f, arena);
            GmoTextureFramesMeasureCopy(tex->tracks, tex->trackCount + 1, f, arena);
            return 1;
        }
    }
    return 1;
}
