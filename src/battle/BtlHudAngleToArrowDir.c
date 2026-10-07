// bdc 0x088306dc BtlHudAngleToArrowDir
#include "bdc.h"

/* Maps a camera-relative bearing `a` to an arrow direction. `a` is first wrapped once by 2*pi:
   above pi it is reduced, at or below -pi it is raised. Returns 0 when it is within 45 degrees
   of 0 (front/up, `BtlHudAngleWithin45`), else 2 within 45 degrees of +pi/2, else 3 within 45
   degrees of -pi/2, else 1 (behind/down). `self` is unused. Used by `BtlHudUpdateEnemyArrow`. */
s32 BtlHudAngleToArrowDir(float a, BtlHud *self)
{
    (void)self;
    if (!(a <= 3.14159274f)) {
        a -= 6.28318548f;
    } else if (a <= -3.14159274f) {
        a += 6.28318548f;
    }
    if (BtlHudAngleWithin45(a, 0.0f) != 0) {
        return 0;
    }
    if (BtlHudAngleWithin45(a, 1.57079637f) != 0) {
        return 2;
    }
    if (BtlHudAngleWithin45(a, -1.57079637f) != 0) {
        return 3;
    }
    return 1;
}
