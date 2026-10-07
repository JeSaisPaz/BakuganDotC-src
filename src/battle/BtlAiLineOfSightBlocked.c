// bdc 0x0888eed4 BtlAiLineOfSightBlocked
#include "bdc.h"

/* Line-of-sight test from unit `from` to unit `to`: copies `from`'s position as the ray origin,
   computes the direction `to.pos - from.pos` (x, y, z; the w lane keeps `to`'s) and returns
   `BtlAiRaycastBlocked``(self, origin, direction, NULL)`: 0 when clear, else the blocking kind (1
   collider, 2/0xff obstacles, 3/4 see `BtlAiUpdateTargetOcclusion`). */
s32 BtlAiLineOfSightBlocked(BtlAi *self, BtlBakugan *from, BtlBakugan *to)
{
    float origin[4];
    float dir[4];

    origin[0] = from->base.pos[0];
    origin[1] = from->base.pos[1];
    origin[2] = from->base.pos[2];
    origin[3] = from->base.pos[3];
    dir[0] = to->base.pos[0] - origin[0];
    dir[1] = to->base.pos[1] - origin[1];
    dir[2] = to->base.pos[2] - origin[2];
    dir[3] = to->base.pos[3];
    return BtlAiRaycastBlocked(self, origin, dir, NULL);
}
