// bdc 0x0889272c BtlAiEvalConditionXZ
#include "bdc.h"

/* Rule-condition evaluator of `BtlAi` for the move and attack rule tables (the
   `MemberFnPtr`s handed to `BtlAiSelectRule`) and the target rules (`BtlAiUpdateTarget`):
   kind 1 compares the horizontal distance to the target (`BtlAiDistanceXZToUnit``(ai, NULL)`)
   with `value * 10` using the comparison ops of `BtlAiEvalCondition` (0 always, 1 `<`, 2 `<=`,
   3 `==`, 4 `!=`, 5 `>=`, 6 `>`, 7 and any other op never; `!=`, `>=` and `>` are true for NaN);
   kind 0x2c is false when the target exists, is in state 0, `value` is non-zero and
   `targetIdleFrames / 30` is below `value`; every other case (kind 0 included) goes to
   `BtlAiEvalCondition`. */
bool BtlAiEvalConditionXZ(float value, BtlAi *self, u8 kind, s32 op)
{
    float dist;
    float limit;

    if (kind < 2) {
        if (kind != 0) {
            dist = BtlAiDistanceXZToUnit(self, NULL);
            limit = value * 10.0f;
            switch (op) {
            case 0:
                return true;
            case 1:
                return dist < limit;
            case 2:
                return dist <= limit;
            case 3:
                return dist == limit;
            case 4:
                return !(dist == limit);
            case 5:
                return !(dist < limit);
            case 6:
                return !(dist <= limit);
            case 7:
            default:
                return false;
            }
        }
    } else if (kind == 0x2c && self->target != NULL && self->target->state == 0 &&
               !(value == 0.0f) && self->targetIdleFrames * 0.0333333351f < value) {
        return false;
    }
    return BtlAiEvalCondition(value, self, kind, op);
}
