// bdc 0x0889042c BtlAiDistanceXZToUnit
#include "bdc.h"

/* Like `BtlAiDistanceToUnit` but horizontal only (Y zeroed): XZ distance between the owner of
   `BtlAi` and `unit` (the current target when NULL), -1.0 when there is none. */
float BtlAiDistanceXZToUnit(BtlAi *self, void *unit)
{
    BtlBakugan *target = unit;
    const float *a;
    const float *b;
    float dx, dz;

    if (target == NULL) {
        target = self->target;
    }
    if (target == NULL) {
        return -1.0f;
    }
    /* |owner->pos - target->pos| over x and z (the y difference is zeroed before the dot) */
    a = self->owner->base.pos;
    b = target->base.pos;
    dx = a[0] - b[0];
    dz = a[2] - b[2];
    return __builtin_sqrtf(dx * dx + dz * dz);
}
