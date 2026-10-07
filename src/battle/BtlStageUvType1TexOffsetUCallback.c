// bdc 0x0889d0b8 BtlStageUvType1TexOffsetUCallback
#include "bdc.h"

/* Material animation callback of `BtlStageSetupUvAnim` (UV-animation type 1): writes GE command
   0x4a (texture offset U) holding `*value` as a 24-bit GE float (the IEEE bits shifted right by 8)
   at `*dl` and advances the display-list pointer. */
void BtlStageUvType1TexOffsetUCallback(u32 **dl, const float *value)
{
    union {
        float f;
        u32 bits;
    } v;

    v.f = *value;
    **dl = (v.bits >> 8) | 0x4a000000;
    *dl = *dl + 1;
}
