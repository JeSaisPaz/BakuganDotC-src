// bdc 0x08862740 BtlBakuganTurnTowardTarget
#include "bdc.h"

/* Turns the unit toward its current target (`BtlBakuganGetTarget`, a model in the Bakugan list)
   with `BtlBakuganTurnToward``(atan2f(target.z - unit.z, target.x - unit.x), 0.3, 0)` and
   returns 1. Returns 0 without turning when there is no target, `noTurn` is set, or `onlyTarget`
   is non-NULL and is not the current target. */
int BtlBakuganTurnTowardTarget(BtlBakugan *self, void *onlyTarget)
{
    GfxModel *target = (GfxModel *)BtlBakuganGetTarget(self);
    float angle;

    if (target == NULL || self->noTurn != 0) {
        return 0;
    }
    if (onlyTarget != NULL && onlyTarget != target) {
        return 0;
    }
    angle = atan2f(target->pos[2] - self->base.pos[2], target->pos[0] - self->base.pos[0]);
    BtlBakuganTurnToward(angle, 0.3f, 0.0f, self);
    return 1;
}
