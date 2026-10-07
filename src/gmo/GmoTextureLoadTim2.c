// bdc 0x08a274e0 GmoTextureLoadTim2
#include "bdc.h"

/* Loads a TIM2 image into the texture object `img` (picture `index` of `data`/`size`): runs the
   measure pass `GmoTim2Measure` against the two-pass arena object (a 0x70-byte stack object:
   `GmoImagePlanInit` init, the measure pass reserves sizes, `GmoImagePlanCommit` allocates,
   `GmoImagePlanFree` finishes), allocates, then builds with `GmoTim2Build`. Returns 1 on success,
   0 when the data is not TIM2 (`TIM2`) or allocation fails. */

s32 GmoTextureLoadTim2(void *img, void *data, u32 size, s32 index)
{
    u8 plan[0x70] __attribute__((aligned(16)));
    s32 result;

    GmoImagePlanInit(plan);
    if (GmoTim2Measure(img, data, size, index, plan) == 0) {
        return 0;
    }
    if (GmoImagePlanCommit(plan) == 0) {
        return 0;
    }
    result = GmoTim2Build(img, data, size, index, plan);
    GmoImagePlanFree(plan);
    return result;
}
