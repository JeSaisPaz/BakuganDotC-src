// bdc 0x0888e1f8 BtlAiDistanceInView
#include "bdc.h"

/* Returns the 3D distance from the owner of `BtlAi` to `pos` when that distance is
   below `maxDist * maxDist` (the binary compares the plain distance with the squared limit) and
   the bearing to `pos` lies within `fovDeg` degrees of the owner's heading `rot[1]`; otherwise 0
   (also with no owner). The bearing difference is wrapped as `d - (int)(d / pi) * 2pi` (as
   compiled), shifted into [0, 2pi) when negative, then folded to `|2pi - d|` from pi upwards or
   `|d|` below pi. */
float BtlAiDistanceInView(float maxDist, float fovDeg, BtlAi *self, float *pos)
{
    BtlBakugan *owner = self->owner;
    float dx;
    float dy;
    float dz;
    float dist;
    float diff;
    float off;

    if (owner == NULL) {
        return 0.0f;
    }
    /* |owner->pos - pos| over x, y, z */
    dx = owner->base.pos[0] - pos[0];
    dy = owner->base.pos[1] - pos[1];
    dz = owner->base.pos[2] - pos[2];
    dist = __builtin_sqrtf(dx * dx + dy * dy + dz * dz);
    if (!(dist < maxDist * maxDist)) {
        return 0.0f;
    }
    diff = atan2f(pos[2] - self->owner->base.pos[2], pos[0] - self->owner->base.pos[0]);
    diff = diff - self->owner->base.rot[1];
    diff = diff - (float)(s32)(diff * 0.318309873f) * 6.28318548f;
    if (diff < 0.0f) {
        diff = diff + 6.28318548f;
    }
    if (diff < 3.14159274f) {
        off = -diff;
    } else {
        off = 6.28318548f - diff;
    }
    if (!(ABS(off) < fovDeg * 0.0174532924f)) {
        return 0.0f;
    }
    return dist;
}
