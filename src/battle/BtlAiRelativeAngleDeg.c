// bdc 0x08890558 BtlAiRelativeAngleDeg
#include "bdc.h"

/* Absolute value of `BtlAiRelativeAngle``(self, from, to)` multiplied by 57.2957802 and then
   by 2 (so the result is twice the angle in degrees); 0 when either unit is NULL. Used by
   condition kinds 0x10/0x16 of `BtlAiEvalCondition` and by `BtlAiIsUnitThreatening`. */
float BtlAiRelativeAngleDeg(BtlAi *self, void *from, void *to)
{
    float angle;

    if (from == NULL || to == NULL) {
        return 0.0f;
    }
    angle = BtlAiRelativeAngle(self, from, to);
    if (angle < 0.0f) {
        angle = -angle;
    }
    return angle * 57.2957802f * 2.0f;
}
