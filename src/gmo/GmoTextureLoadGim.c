// bdc 0x08a26c20 GmoTextureLoadGim
#include "bdc.h"

/* Loads a GIM image into the texture object `img` (picture `index` of `data`/`size`): runs the
   measure pass `GmoGimMeasure` against the two-pass arena object (a 0x70-byte stack object:
   `GmoImagePlanInit` init, the measure pass reserves sizes, `GmoImagePlanCommit` allocates,
   `GmoImagePlanFree` finishes), allocates, then builds with `GmoGimBuild`. Returns 1 on success,
   0 when the data is not GIM (`MIG.00.1PSP`) or allocation fails. */

s32 GmoTextureLoadGim(void *img, void *data, u32 size, s32 index) {
    u8 plan[0x70];
    s32 ok;

    GmoImagePlanInit(plan);
    ok = GmoGimMeasure(img, data, size, index, plan);
    if (ok != 0 && GmoImagePlanCommit(plan) != 0) {
        ok = GmoGimBuild(img, data, size, index, plan);
        GmoImagePlanFree(plan);
        return ok;
    }
    return 0;
}
