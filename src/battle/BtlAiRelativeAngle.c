// bdc 0x08890474 BtlAiRelativeAngle
#include "bdc.h"

/* Signed angle (radians) between the facing of unit `from` (yaw `rot[1]`) and the direction from
   `from` to `to` (`atan2f` of the XZ position delta); 0 when either is NULL. The difference
   `yaw - heading` is reduced by `2pi * trunc(d / pi)`, raised by 2pi when negative, then returned
   as `-d` when below pi and `2pi - d` otherwise. Used by `BtlAiUpdate` and
   `BtlAiRelativeAngleDeg`. */
float BtlAiRelativeAngle(BtlAi *self, BtlBakugan *from, BtlBakugan *to)
{
    float d;
    float toX;
    float toZ;

    (void)self;
    if (from == NULL || to == NULL) {
        return 0.0f;
    }
    /* quad copy of to's position; only x and z are used */
    toX = to->base.pos[0];
    toZ = to->base.pos[2];
    d = from->base.rot[1] - atan2f(toZ - from->base.pos[2], toX - from->base.pos[0]);
    d = d - (float)(s32)(d * 0.318309873f) * 6.28318548f;
    if (d < 0.0f) {
        d = d + 6.28318548f;
    }
    if (d < 3.14159274f) {
        return -d;
    }
    return 6.28318548f - d;
}
