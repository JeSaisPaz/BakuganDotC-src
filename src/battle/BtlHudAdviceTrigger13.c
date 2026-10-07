// bdc 0x08839508 BtlHudAdviceTrigger13
#include "bdc.h"

/* Advice trigger of slot 13: only after slot 12 has a message (`adviceMsg[12] != -1`) and while
   this slot has none. Once the unit's statistics counters 3+4+5 (`BtlStatsGetCounter`, 0 without
   a statistics record) reach 40, marks the slot as checked (`adviceMsg[slot] = 0`) and fires when
   counters 0+1+2 sum to zero. The message comes from `BtlHudAdvicePickMessage` (normal line);
   always ends with `BtlHudAdviceStep``(self, fire, msgId, slot)` (msgId -1 when not firing). */

void BtlHudAdviceTrigger13(BtlHud *self, void *unit, int slot)
{
    BtlBakugan *bakugan = unit;
    bool fire = false;
    int msgId = -1;
    int c0, c1, c2;

    if (self->adviceMsg[12] != -1 && self->adviceMsg[slot] == -1) {
        c0 = bakugan->stats != NULL ? BtlStatsGetCounter(bakugan->stats, 3) : 0;
        c1 = bakugan->stats != NULL ? BtlStatsGetCounter(bakugan->stats, 4) : 0;
        c2 = bakugan->stats != NULL ? BtlStatsGetCounter(bakugan->stats, 5) : 0;
        if (c0 + c1 + c2 >= 40) {
            self->adviceMsg[slot] = 0;
            c0 = bakugan->stats != NULL ? BtlStatsGetCounter(bakugan->stats, 0) : 0;
            c1 = bakugan->stats != NULL ? BtlStatsGetCounter(bakugan->stats, 1) : 0;
            c2 = bakugan->stats != NULL ? BtlStatsGetCounter(bakugan->stats, 2) : 0;
            if (c0 + c1 + c2 == 0) {
                fire = true;
            }
        }
    }
    if (fire) {
        msgId = BtlHudAdvicePickMessage(self, slot, false);
    }
    BtlHudAdviceStep(self, fire, msgId, slot);
}
