// bdc 0x08a24e60 GmoTextureArrayMeasureClone
#include "bdc.h"

/* Measure pass for copying `count` texture objects (0x40-byte records): when a copy is needed
   (`GmoTextureNeedsClone`) reserves the records (`GmoImagePlanReserveTextures`) and measures each
   with `GmoTextureMeasureClone`. Returns 1 (0 for NULL arguments). */

s32 GmoTextureArrayMeasureClone(void *images, s32 count, u32 flags, void *arena)
{
    GmoTexture *tex = (GmoTexture *)images;
    s32 i;

    if (tex == NULL || arena == NULL) {
        return 0;
    }
    if (GmoTextureNeedsClone(tex, count, flags) != 0) {
        GmoImagePlanReserveTextures(count, arena);
        for (i = 0; i < count; i++) {
            GmoTextureMeasureClone(NULL, &tex[i], flags, arena);
        }
    }
    return 1;
}
