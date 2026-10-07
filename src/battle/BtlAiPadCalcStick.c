// bdc 0x08899220 BtlAiPadCalcStick
#include "bdc.h"

/* Turns the stick axes of the CPU AI's virtual pad into a move request: magnitude
   `(x*x + y*y - 0.3) * 1/0.7` (dot of `{-stickY, 0, -stickX}`) in `stickMagnitude`; when it
   is not <= 0.5, stores the heading `pi - atan2f(-stickX, -stickY)` wrapped into (-pi, pi] in
   `stickHeading` and returns 1, else zeroes the magnitude and returns 0. */
s32 BtlAiPadCalcStick(BtlAiPad *self)
{
    float dir[4];
    float lenSq;
    float mag;
    float heading;

    /* The binary also copies `vec` to an unused stack slot (lv.q/sv.q); omitted. */
    dir[0] = -self->stickY;
    dir[1] = 0.0f;
    dir[2] = -self->stickX;
    dir[3] = 0.0f;
    lenSq = dir[0] * dir[0] + dir[1] * dir[1] + dir[2] * dir[2];
    mag = (lenSq - 0.300000012f) * 1.42857146f;
    self->stickMagnitude = mag;
    if (mag <= 0.5f) {
        self->stickMagnitude = 0.0f;
        return 0;
    }
    heading = 3.14159274f - atan2f(dir[2], dir[0]);
    if (heading <= 3.14159274f) {
        if (heading <= -3.14159274f) {
            heading = heading + 6.28318548f;
        }
    } else {
        heading = heading - 6.28318548f;
    }
    self->stickHeading = heading;
    return 1;
}
