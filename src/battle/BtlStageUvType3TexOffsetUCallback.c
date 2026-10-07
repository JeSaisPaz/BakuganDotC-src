// bdc 0x0889d110 BtlStageUvType3TexOffsetUCallback
#include "bdc.h"

/* Material animation callback of `BtlStageSetupUvAnim` (UV-animation type 3): writes GE command
   `0x4a` (texture offset U) = the raw bits of the float `*value` shifted to a 24-bit GE float
   (`bits >> 8`) at `*dl` and advances the display-list pointer. */
void BtlStageUvType3TexOffsetUCallback(u32 **dl, const float *value)
{
    union {
        float f;
        u32 bits;
    } v;

    v.f = *value;
    **dl = (v.bits >> 8) | 0x4a000000;
    *dl = *dl + 1;
}
