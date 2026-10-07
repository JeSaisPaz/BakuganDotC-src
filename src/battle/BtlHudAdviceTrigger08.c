// bdc 0x08839014 BtlHudAdviceTrigger08
#include "bdc.h"

/* Advice trigger (slot 8 when called by BtlHudUpdateAdvice): fires when no message is pending in
   `slot` (adviceMsg[slot] == -1) and some standing stage object is below 40% HP
   (ActorStageObjAnyTargetBelowHp(0.4)); a fired trigger picks the normal line for `slot`. Always
   steps the slot's advice state machine. `unit` is unused. */

void BtlHudAdviceTrigger08(BtlHud *self, void *unit, int slot)
{
    bool fire = false;
    int msgId = -1;

    (void)unit;
    if (self->adviceMsg[slot] == -1 && ActorStageObjAnyTargetBelowHp(0.400000006f) != 0) {
        fire = true;
    }
    if (fire) {
        msgId = BtlHudAdvicePickMessage(self, slot, false);
    }
    BtlHudAdviceStep(self, fire, msgId, slot);
}
