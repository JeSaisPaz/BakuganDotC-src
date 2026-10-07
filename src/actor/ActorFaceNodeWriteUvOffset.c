// bdc 0x088dbf94 ActorFaceNodeWriteUvOffset
#include "bdc.h"

/* Face-node display-list callback bound by `ActorSetFaceExpression` to the eye/eyebrow/mouth
   nodes: appends GE `TOFFSETU` = `uv[0]` and `TOFFSETV` = `uv[1]` to the display list `*dl`,
   selecting the expression cell of the face texture. The float bit patterns are shifted right
   by 8 (the GE takes the upper 24 bits of the float). */

void ActorFaceNodeWriteUvOffset(u32 **dl, const float *uv)
{
    const u32 *bits = (const u32 *)uv;
    u32 u = bits[0];
    u32 v = bits[1];
    u32 *cmd = *dl;

    cmd[0] = (u >> 8) | 0x4a000000;
    cmd[1] = (v >> 8) | 0x4b000000;
    *dl = cmd + 2;
}
