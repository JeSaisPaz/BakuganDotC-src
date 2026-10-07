// bdc 0x088390b8 BtlHudAdviceTrigger09
#include "bdc.h"

/* HUD advice trigger of slot 9: while no message is stored for the slot
   (`adviceMsg[slot] == -1`), fires when the number of standing attribute landmarks
   (`ActorStageObjCountStandingAttrLandmarks`) is below `adviceThreshold` (signed compare); on
   firing it calls `BtlHudAdviceGetFace` (result unused) and picks the message with
   `BtlHudAdvicePickMessage` (normal line). Always runs `BtlHudAdviceStep` with the result
   (`msgId` -1 when not fired). `unit` is unused. */
void BtlHudAdviceTrigger09(BtlHud *self, void *unit, int slot)
{
    bool fire = false;
    int msgId = -1;

    (void)unit;
    if (self->adviceMsg[slot] == -1 &&
        ActorStageObjCountStandingAttrLandmarks() < self->adviceThreshold) {
        fire = true;
    }
    if (fire) {
        BtlHudAdviceGetFace();
        msgId = BtlHudAdvicePickMessage(self, slot, false);
    }
    BtlHudAdviceStep(self, fire, msgId, slot);
}
