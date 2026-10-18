// bdc 0x08864080 BtlBakuganClassifyElevationToPoint
#include "bdc.h"

/* Returns the elevation class of the unit's position seen from `point`: with `d = pos - point`,
   the angle `atan2f(d.y, |d.xz|)` gives 5 unless it is <= 0.314159274 rad (18°; NaN also gives
   5), else 4 when it is below -0.314159274, else 1. */
int BtlBakuganClassifyElevationToPoint(BtlBakugan *self, float *point)
{
    float dx, dy, dz;
    float horizontal;
    float angle;

    dx = VfSub(self->base.pos[0], point[0]);
    dy = VfSub(self->base.pos[1], point[1]);
    dz = VfSub(self->base.pos[2], point[2]);
    /* vdot.t with the y lane zeroed, then vsqrt.s */
    horizontal = VfSqrt(VfDot3(dx, 0.0f, dz, dx, 0.0f, dz));
    angle = atan2f(dy, horizontal);
    if (!(angle <= 0.314159274f)) {
        return 5;
    }
    if (angle < -0.314159274f) {
        return 4;
    }
    return 1;
}
