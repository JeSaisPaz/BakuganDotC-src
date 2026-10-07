// bdc 0x0888e368 BtlAiDistanceInViewToUnit
#include "bdc.h"

/* `BtlAiDistanceInView` for a copy of `unit`'s position (`base.pos`, a 16-byte quad copy);
   returns 0 when `unit` is NULL. */
float BtlAiDistanceInViewToUnit(float maxDist, float fovDeg, BtlAi *self, BtlBakugan *unit)
{
    float pos[4];

    if (unit == NULL) {
        return 0.0f;
    }
    pos[0] = unit->base.pos[0];
    pos[1] = unit->base.pos[1];
    pos[2] = unit->base.pos[2];
    pos[3] = unit->base.pos[3];
    return BtlAiDistanceInView(maxDist, fovDeg, self, pos);
}
