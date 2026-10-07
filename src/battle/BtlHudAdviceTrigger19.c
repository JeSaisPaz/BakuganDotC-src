// bdc 0x08839ca8 BtlHudAdviceTrigger19
#include "bdc.h"

/* Advice trigger for HUD advice slot `slot` (19 from `BtlHudUpdateAdvice`): fires when the slot
   has no pending message (`adviceMsg[slot] == -1`) and the player Bakugan `unit` has had an art
   ready to fire for more than 0x30b (779) frames (`combat.artReadyFrames`, `+0x534`); then picks
   the message with `BtlHudAdvicePickMessage` (normal line), else passes -1. Always steps the
   slot's state machine with `BtlHudAdviceStep`. */

void BtlHudAdviceTrigger19(BtlHud *self, BtlBakugan *unit, int slot)
{
    bool fire;
    int msgId;

    fire = false;
    if (self->adviceMsg[slot] == -1 && unit->combat.artReadyFrames > 0x30b) {
        fire = true;
    }
    msgId = -1;
    if (fire) {
        msgId = BtlHudAdvicePickMessage(self, slot, false);
    }
    BtlHudAdviceStep(self, fire, msgId, slot);
}
