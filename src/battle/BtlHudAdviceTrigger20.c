// bdc 0x08839d3c BtlHudAdviceTrigger20
#include "bdc.h"

/* Advice trigger for HUD advice slot `slot` (20 from `BtlHudUpdateAdvice`): while the slot has no
   message (`adviceMsg[slot] == -1`) and the HUD advice frame counter `adviceFrame` (`+0xbc`)
   exceeds 0x30c (780), sets `adviceMsg[slot]` to 0 (so the test runs only once) and fires when the
   player Bakugan `unit` has an equipped art charged below half
   (`BtlCombatHasArtBelowCharge``(0.5, &unit->combat)`); then picks the message with
   `BtlHudAdvicePickMessage` (normal line), else passes -1. Always steps the slot's state machine
   with `BtlHudAdviceStep`. */

void BtlHudAdviceTrigger20(BtlHud *self, BtlBakugan *unit, int slot)
{
    bool fire;
    int msgId;

    fire = false;
    msgId = -1;
    if (self->adviceMsg[slot] == -1 && self->adviceFrame > 0x30c) {
        self->adviceMsg[slot] = 0;
        if (BtlCombatHasArtBelowCharge(0.5f, &unit->combat) != 0) {
            fire = true;
        }
    }
    if (fire) {
        msgId = BtlHudAdvicePickMessage(self, slot, false);
    }
    BtlHudAdviceStep(self, fire, msgId, slot);
}
