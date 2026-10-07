// bdc 0x08a28cf0 GmoTextureLoadBmp
#include "bdc.h"

/* Loads a BMP image into the texture object `img` (picture `index` of `data`/`size`): runs the
   measure pass `GmoBmpMeasure` against the two-pass arena object (a 0x70-byte stack object:
   `GmoImagePlanInit` init, the measure pass reserves sizes, `GmoImagePlanCommit` allocates,
   `GmoImagePlanFree` finishes), allocates, then builds with `GmoBmpBuild`. Returns 1 on success,
   0 when the data is not BMP (`BM`) or allocation fails. */

s32 GmoTextureLoadBmp(void *img, void *data, u32 size, s32 index)
{
    u8 plan[0x70] __attribute__((aligned(16)));
    s32 result;

    GmoImagePlanInit(plan);
    if (GmoBmpMeasure(img, data, size, index, plan) == 0) {
        return 0;
    }
    if (GmoImagePlanCommit(plan) == 0) {
        return 0;
    }
    result = GmoBmpBuild(img, data, size, index, plan);
    GmoImagePlanFree(plan);
    return result;
}
