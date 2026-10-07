// bdc 0x08a25a78 GmoTextureMeasureWritable
#include "bdc.h"

/* Measure pass of `GmoTextureMakeWritable`: when `flags & 2`, reserves private copies of the
   pixel image list (`images`) and palette list (`palettes`) that are not already owned (no id-1
   entry: `GmoImageArrayMeasureCopy`, `GmoPaletteArrayMeasureCopy` with flags `0x80000001`)
   and, when the record still uses its default command list (`image == palette`) and flag bit 0x10
   is clear, the space for a new GE list (`GmoTextureWriteDl` size + 4) in pool 2. Returns 1
   (0 for NULL). */

s32 GmoTextureMeasureWritable(void *texp, u32 flags, void *plan)
{
    GmoTexture *tex = (GmoTexture *)texp;
    GmoImage *ownImage;
    GmoImage *ownPalette;
    GmoImage *palettes;

    if (tex == NULL) {
        return 0;
    }
    if ((flags & 2) == 0) {
        return 1;
    }
    ownImage = GmoTextureFindImage(tex, 1, 0);
    ownPalette = GmoTextureFindPalette(tex, 1, 0);
    palettes = tex->palettes;
    if (tex->images != NULL && ownImage == NULL) {
        GmoImageArrayMeasureCopy(tex->images, 1, 0x80000001, plan);
    }
    if (palettes != NULL && ownPalette == NULL) {
        GmoPaletteArrayMeasureCopy(palettes, 1, 0x80000001, plan);
    }
    if (tex->image == tex->palette && (tex->flags & 0x10) == 0) {
        s32 size = GmoTextureWriteDl(tex, NULL, NULL, 0x80000001);
        GmoImagePlanReserve(plan, 2, 4, size + 4);
    }
    return 1;
}
