// bdc 0x08884b1c BtlInputReadRemoteStick
#include "bdc.h"

/* Network variant of `BtlInputReadLocalStick` for stick values `x`/`y` received from the remote
   player: stores `{x, 0, y, 0}` in `stick` and the magnitude `(x*x + y*y - 0.3) * 1/0.7` (VFPU dot)
   in `stickMagnitude`. When that is not <= 0.5 it stores the heading `pi - atan2f(y, x)` (wrapped
   into (-pi, pi]) plus `BtlInputYawToOffset``(cameraYaw)` in `stickHeading`, rebuilds `dir` as
   `{cos, 0, sin, 0}` of `stickHeading` (VFPU `vrot` of `stickHeading * 2/pi` quarter turns),
   stores the 3D dot of the new and previous `dir` in `headingDot` and returns 1; otherwise zeroes
   `stickMagnitude` and returns 0. Used by `BtlInputReadActions` in network play. */
int BtlInputReadRemoteStick(float x, float y, float cameraYaw, BtlInput *self)
{
    float prevDir[4];
    float raw[4];
    float mag;
    float heading;
    float angle;

    prevDir[0] = self->dir[0];
    prevDir[1] = self->dir[1];
    prevDir[2] = self->dir[2];
    prevDir[3] = self->dir[3];
    raw[0] = x;
    raw[1] = 0.0f;
    raw[2] = y;
    raw[3] = 0.0f;
    self->stick[0] = raw[0];
    self->stick[1] = raw[1];
    self->stick[2] = raw[2];
    self->stick[3] = raw[3];
    mag = ((raw[0] * raw[0] + raw[1] * raw[1] + raw[2] * raw[2]) - 0.300000012f) * 1.42857146f; /* 0x3e99999a, 0x3fb6db6e */
    self->stickMagnitude = mag;
    if (mag <= 0.5f) {
        self->stickMagnitude = 0.0f;
        return 0;
    }
    heading = 3.14159274f - atan2f(raw[2], raw[0]); /* 0x40490fdb */
    if (heading <= 3.14159274f) {
        if (heading <= -3.14159274f) {
            heading = heading + 6.28318548f; /* 0x40c90fdb */
        }
    } else {
        heading = heading - 6.28318548f;
    }
    angle = heading + BtlInputYawToOffset(cameraYaw);
    self->stickHeading = angle;
    /* vrot.q [C, 0, S, 0] of angle * S703 (2/pi) quarter turns */
    self->dir[0] = __builtin_cosf(angle);
    self->dir[1] = 0.0f;
    self->dir[2] = __builtin_sinf(angle);
    self->dir[3] = 0.0f;
    self->headingDot = self->dir[0] * prevDir[0] + self->dir[1] * prevDir[1] + self->dir[2] * prevDir[2];
    return 1;
}
