// bdc 0x0889d13c BtlStageUvType4TexOffsetVCallback
#include "bdc.h"

/* Material animation callback of `BtlStageSetupUvAnim` (UV-animation type 4): writes GE command
   0x4b (texture offset V) holding `*value` as a 24-bit GE float (the IEEE bits shifted right by 8)
   at `*dl` and advances the display-list pointer. */
void BtlStageUvType4TexOffsetVCallback(u32 **dl, const float *value)
{
    union {
        float f;
        u32 bits;
    } v;

    v.f = *value;
    **dl = (v.bits >> 8) | 0x4b000000;
    *dl = *dl + 1;
}
