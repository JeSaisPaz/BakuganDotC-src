// bdc 0x08838dac BtlHudAdviceTrigger05
#include "bdc.h"

/* Advice trigger of slot 5: fires while the slot has no message (`adviceMsg[slot] == -1`) and the
   unit's HP ratio (`BtlCombatGetHpRatio` on its combat state) is ≤ 0.5 (false for NaN); the
   message is picked by `BtlHudAdvicePickMessage` with the alternate line when the unit has a
   statistics record whose counter 0xf (`BtlStatsGetCounter`) is > 0. Always ends with
   `BtlHudAdviceStep``(self, fire, msgId, slot)` (msgId -1 when not firing). */

void BtlHudAdviceTrigger05(BtlHud *self, void *unit, int slot)
{
    BtlBakugan *bakugan = unit;
    bool fire = false;
    bool useAltLine;
    int msgId = -1;

    if (self->adviceMsg[slot] == -1 && BtlCombatGetHpRatio(&bakugan->combat) <= 0.5f) {
        fire = true;
    }
    if (fire) {
        useAltLine = false;
        if (bakugan->stats != NULL) {
            useAltLine = BtlStatsGetCounter(bakugan->stats, 0xf) > 0;
        }
        msgId = BtlHudAdvicePickMessage(self, slot, useAltLine);
    }
    BtlHudAdviceStep(self, fire, msgId, slot);
}
