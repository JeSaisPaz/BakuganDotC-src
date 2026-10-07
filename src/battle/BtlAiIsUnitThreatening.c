// bdc 0x08892c58 BtlAiIsUnitThreatening
#include "bdc.h"

/* Threat test of `BtlAi` against `unit` (a `BtlBakugan`): returns 0 for NULL or
   when its distance (`BtlAiDistanceToUnit`) exceeds `threatRange` of `g_btlAiRangeConsts`.
   If `unit` targets the owner (`BtlBakuganGetTarget`) it is a threat while in state 7 or 9;
   otherwise when `BtlAiRelativeAngleDeg``(unit, owner)` (doubled degrees) is below 120 and the
   XZ distance (`BtlAiDistanceXZToUnit`) is below 300. Returns 1 for a threat, else 0. */
s32 BtlAiIsUnitThreatening(BtlAi *self, void *unit)
{
    BtlBakugan *other = (BtlBakugan *)unit;
    float angle;
    float distXZ;

    if (other == NULL) {
        return 0;
    }
    if (g_btlAiRangeConsts.threatRange < BtlAiDistanceToUnit(self, other)) {
        return 0;
    }
    if (BtlBakuganGetTarget(other) == self->owner) {
        return (other->state == 7 || other->state == 9) ? 1 : 0;
    }
    angle = BtlAiRelativeAngleDeg(self, other, self->owner);
    distXZ = BtlAiDistanceXZToUnit(self, other);
    if (angle < 120.0f && distXZ < 300.0f) {
        return 1;
    }
    return 0;
}
