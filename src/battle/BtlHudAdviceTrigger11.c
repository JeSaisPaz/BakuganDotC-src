// bdc 0x08839278 BtlHudAdviceTrigger11
#include "bdc.h"

/* Advice trigger of slot 11: once slot 10 has been checked (`adviceMsg[10] != -1`), when this slot
   has no message and the unit's HP ratio (`BtlCombatGetHpRatio`) is <= 0.35 (false for NaN),
   marks the slot checked (`adviceMsg[slot] = 0`) and fires if the unit's statistics counter 0x15
   (`BtlStatsGetCounter`) is > 9 (no record: no fire) and counter 0xb is < 5 (no record:
   passes). A firing trigger picks its message with `BtlHudAdvicePickMessage` (main line);
   always ends with `BtlHudAdviceStep``(self, fire, msgId, slot)` (msgId -1 when not firing). */

void BtlHudAdviceTrigger11(BtlHud *self, void *unit, int slot)
{
    BtlBakugan *bakugan = unit;
    bool fire = false;
    bool pass;
    int msgId = -1;

    if (self->adviceMsg[10] != -1 && self->adviceMsg[slot] == -1 &&
        BtlCombatGetHpRatio(&bakugan->combat) <= 0.349999994f) {
        self->adviceMsg[slot] = 0;
        pass = false;
        if (bakugan->stats != NULL) {
            pass = BtlStatsGetCounter(bakugan->stats, 0x15) > 9;
        }
        if (pass) {
            pass = true;
            if (bakugan->stats != NULL) {
                pass = BtlStatsGetCounter(bakugan->stats, 0xb) < 5;
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
