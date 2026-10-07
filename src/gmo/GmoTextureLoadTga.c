// bdc 0x08a27e74 GmoTextureLoadTga
#include "bdc.h"

/* Loads a TGA image into the texture object `img` (picture `index` of `data`/`size`): runs the
   measure pass `GmoTgaMeasure` against the two-pass arena object (a 0x70-byte stack object:
   `GmoImagePlanInit` init, the measure pass reserves sizes, `GmoImagePlanCommit` allocates,
   `GmoImagePlanFree` finishes), allocates, then builds with `GmoTgaBuild`. Returns 1 on success,
   0 when the data is not TGA (image type 1/2/9/10) or allocation fails. */

s32 GmoTextureLoadTga(void *img, void *data, u32 size, s32 index)
{
    u8 plan[0x70] __attribute__((aligned(16)));
    s32 result;

    GmoImagePlanInit(plan);
    if (GmoTgaMeasure(img, data, size, index, plan) == 0) {
        return 0;
    }
    if (GmoImagePlanCommit(plan) == 0) {
        return 0;
    }
    result = GmoTgaBuild(img, data, size, index, plan);
    GmoImagePlanFree(plan);
    return result;
}
