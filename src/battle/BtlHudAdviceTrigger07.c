// bdc 0x08838f74 BtlHudAdviceTrigger07
#include "bdc.h"

/* HUD advice trigger of slot 7: while no message is stored for the slot
   (`adviceMsg[slot] == -1`), fires when a standing script-created HP object is below 75 % HP
   (`ActorStageObjAnyTargetBelowHp``(0.75)`), picking the message with
   `BtlHudAdvicePickMessage` (normal line); always runs `BtlHudAdviceStep` with the result
   (`msgId` -1 when not fired). `unit` is unused. */
void BtlHudAdviceTrigger07(BtlHud *self, void *unit, int slot)
{
    bool fire = false;
    int msgId = -1;

    (void)unit;
    if (self->adviceMsg[slot] == -1 && ActorStageObjAnyTargetBelowHp(0.75f) != 0) {
        fire = true;
    }
    if (fire) {
        msgId = BtlHudAdvicePickMessage(self, slot, false);
    }
    BtlHudAdviceStep(self, fire, msgId, slot);
}
