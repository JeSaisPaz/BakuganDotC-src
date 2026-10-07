// bdc 0x08892908 BtlAiEvalRuleCondition
#include "bdc.h"

/* Evaluates the current condition of an AI command channel: kind `condKind`, value `condValue`,
   comparison `condOp`. Kind 3 returns the channel's `cmdExpired` byte, kind 0x2c is true when
   `pendingCmd` is 0xe, kinds 1 and 2 compare the target distance (`BtlAiDistanceToUnit`) or
   the AI's `odometer` with `condValue * 10` (op 0 true, 1 `<`, 2 `<=`, 3 `==`, 4 `!=`, 5 `!(<)`,
   6 `!(<=)`, 7 and others false; the negated forms are true for NaN); every other kind (and
   kind 0x2c without command 0xe) goes to `BtlAiEvalCondition`. */

bool BtlAiEvalRuleCondition(BtlAi *self, BtlAiChannel *channel)
{
    u8 kind;
    float measured;
    float limit;

    kind = channel->condKind;
    if (kind == 0x2c) {
        if (channel->pendingCmd == 0xe) {
            return true;
        }
    } else if (kind == 3) {
        return channel->cmdExpired;
    } else if (kind == 2 || kind == 1) {
        if (kind == 2) {
            measured = self->odometer;
        } else {
            measured = BtlAiDistanceToUnit(self, NULL);
        }
        limit = channel->condValue * 10.0f;
        switch (channel->condOp) {
        case 0:
            return true;
        case 1:
            return measured < limit;
        case 2:
            return measured <= limit;
        case 3:
            return measured == limit;
        case 4:
            return !(measured == limit);
        case 5:
            return !(measured < limit);
        case 6:
            return !(measured <= limit);
        default:
            return false;
        }
    }
    return BtlAiEvalCondition(channel->condValue, self, kind, channel->condOp);
}
