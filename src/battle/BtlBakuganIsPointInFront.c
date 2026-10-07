// bdc 0x08864144 BtlBakuganIsPointInFront
#include "bdc.h"

/* Returns 1 when `point` lies in front of the unit: the angle between its heading `rot[1]` and
   `atan2f(unit.z - p.z, unit.x - p.x)` (the direction from `point` towards the unit) is wrapped
   to [0, 2pi) (the wrap subtracts `(int)(d / pi) * 2pi`, then adds 2pi when negative), folded to
   its shortest magnitude, and the result is 1 unless that magnitude is <= pi/2 (so also 1 for
   NaN). */
int BtlBakuganIsPointInFront(BtlBakugan *self, float *point)
{
    float heading = self->base.rot[1];
    float pos[4];
    float diff;
    float angle;

    pos[0] = self->base.pos[0];
    pos[1] = self->base.pos[1];
    pos[2] = self->base.pos[2];
    pos[3] = self->base.pos[3];
    diff = heading - atan2f(pos[2] - point[2], pos[0] - point[0]);
    diff = diff - (float)(s32)(diff * 0.318309873f) * 6.28318548f;
    if (diff < 0.0f) {
        diff = diff + 6.28318548f;
    }
    if (diff < 3.14159274f) {
        angle = -diff;
    } else {
        angle = 6.28318548f - diff;
    }
    return !(ABS(angle) <= 1.57079637f);
}
