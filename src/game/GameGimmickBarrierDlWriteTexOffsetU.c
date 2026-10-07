// bdc 0x088d9854 GameGimmickBarrierDlWriteTexOffsetU
#include "bdc.h"

/* Material animation callback bound by `GameGimmickBarrierBindNode` to the barrier material
   `gfx_102m__BA_CN` (value = vec4 at gimmick `+0x180`): appends GE command 0x4A (texture U offset)
   with `*value` to `*dl`, scrolling the barrier texture. */
void GameGimmickBarrierDlWriteTexOffsetU(u32 **dl, const float *value)
{
    union {
        float f;
        u32 bits;
    } v;

    v.f = *value;
    **dl = (v.bits >> 8) | 0x4a000000;
    *dl = *dl + 1;
}
