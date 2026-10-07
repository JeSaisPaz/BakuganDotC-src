// bdc 0x088bdf00 GameFieldTvDlWriteTexOffsetU
#include "bdc.h"

/* Material animation callback bound by `GameFieldPhaseLoad` to the `tv_img_02`/`tv_img_03`
   materials of the field stage model: writes GE command 0x4a (texture offset U) holding `*value`
   as a 24-bit GE float (the IEEE bits shifted right by 8) at `*dl` and advances the display-list
   pointer, scrolling the TV images. */
void GameFieldTvDlWriteTexOffsetU(u32 **dl, const float *value)
{
    union {
        float f;
        u32 bits;
    } v;

    v.f = *value;
    **dl = (v.bits >> 8) | 0x4a000000;
    *dl = *dl + 1;
}
