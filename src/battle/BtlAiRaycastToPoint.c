// bdc 0x08893178 BtlAiRaycastToPoint
#include "bdc.h"

/* Casts from the owner of `BtlAi` to `point` with `BtlAiRaycastBlocked` (origin
   a copy of the owner's position quad, direction `point - pos` in x, y, z with `point`'s w lane)
   but discards the result and always returns 1. */
s32 BtlAiRaycastToPoint(BtlAi *self, float *point)
{
    float origin[4];
    float dir[4];
    float *ownerPos = self->owner->base.pos;

    origin[0] = ownerPos[0];
    origin[1] = ownerPos[1];
    origin[2] = ownerPos[2];
    origin[3] = ownerPos[3];
    dir[0] = point[0] - origin[0];
    dir[1] = point[1] - origin[1];
    dir[2] = point[2] - origin[2];
    dir[3] = point[3];
    BtlAiRaycastBlocked(self, origin, dir, NULL);
    return 1;
}
