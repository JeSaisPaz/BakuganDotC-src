// bdc 0x08839164 BtlHudAdviceTrigger10
#include "bdc.h"

/* Advice trigger of slot 10: when the slot has no message (`adviceMsg[slot] == -1`) and the unit's
   HP ratio (`BtlCombatGetHpRatio`) is <= 0.7 (false for NaN), marks the slot checked
   (`adviceMsg[slot] = 0`) and fires if the unit's statistics counter 0x15 (`BtlStatsGetCounter`)
   is > 4 (no record: no fire) and counter 0xb is < 2 (no record: passes). A firing trigger picks
   its message with `BtlHudAdvicePickMessage` (main line); always ends with
   `BtlHudAdviceStep``(self, fire, msgId, slot)` (msgId -1 when not firing). */

void BtlHudAdviceTrigger10(BtlHud *self, void *unit, int slot)
{
    BtlBakugan *bakugan = unit;
    bool fire = false;
    bool pass;
    int msgId = -1;

    if (self->adviceMsg[slot] == -1 && BtlCombatGetHpRatio(&bakugan->combat) <= 0.699999988f) {
        self->adviceMsg[slot] = 0;
        pass = false;
        if (bakugan->stats != NULL) {
            pass = BtlStatsGetCounter(bakugan->stats, 0x15) > 4;
        }
        if (pass) {
            pass = true;
            if (bakugan->stats != NULL) {
                pass = BtlStatsGetCounter(bakugan->stats, 0xb) < 2;
            }
            if (pass) {
                fire = true;
            }
        }
    }
    if (fire) {
        msgId = BtlHudAdvicePickMessage(self, slot, false);
    }
    BtlHudAdviceStep(self, fire, msgId, slot);
}
