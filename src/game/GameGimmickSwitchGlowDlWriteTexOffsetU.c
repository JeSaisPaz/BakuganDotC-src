// bdc 0x088db39c GameGimmickSwitchGlowDlWriteTexOffsetU
#include "bdc.h"

/* Material animation callback bound by `GameGimmickSwitchBindGlowNode` to the switch glow
   material `fz_quest_switch01_04` (value = vec4 at gimmick `+0x180`): appends GE command 0x4A
   (texture U offset) with `*value` to `*dl`, scrolling the glow texture. */
void GameGimmickSwitchGlowDlWriteTexOffsetU(u32 **dl, const float *value)
{
    union {
        float f;
        u32 bits;
    } v;

    v.f = *value;
    **dl = (v.bits >> 8) | 0x4a000000;
    *dl = *dl + 1;
}
