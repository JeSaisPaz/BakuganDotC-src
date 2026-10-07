// bdc 0x088397fc BtlHudAdviceTrigger15
#include "bdc.h"

/* Advice trigger of slot 15, the follow-up of slot 14: only once slot 14 holds a message
   (`adviceMsg[14] != -1`) and while this slot has none (`adviceMsg[slot] == -1`). When the unit's
   statistics counters 0+1+2 (`BtlStatsGetCounter`; 0 without a statistics record) reach 40, the
   slot is marked checked (`adviceMsg[slot] = 0`, so it never tests again) and it fires when
   counters 3+4+5 are all zero (sum 0). The message is picked by `BtlHudAdvicePickMessage`
   (normal line). Always ends with `BtlHudAdviceStep``(self, fire, msgId, slot)` (msgId -1 when
   not firing). */

void BtlHudAdviceTrigger15(BtlHud *self, void *unit, int slot)
{
    BtlBakugan *bakugan = unit;
    bool fire = false;
    int msgId = -1;
    int a;
    int b;
    int c;

    if (self->adviceMsg[14] != -1 && self->adviceMsg[slot] == -1) {
        a = 0;
        if (bakugan->stats != NULL) {
            a = BtlStatsGetCounter(bakugan->stats, 0);
        }
        b = 0;
        if (bakugan->stats != NULL) {
            b = BtlStatsGetCounter(bakugan->stats, 1);
        }
        c = 0;
        if (bakugan->stats != NULL) {
            c = BtlStatsGetCounter(bakugan->stats, 2);
        }
        if (a + b + c >= 40) {
            self->adviceMsg[slot] = 0;
            a = 0;
            if (bakugan->stats != NULL) {
                a = BtlStatsGetCounter(bakugan->stats, 3);
            }
            b = 0;
            if (bakugan->stats != NULL) {
                b = BtlStatsGetCounter(bakugan->stats, 4);
            }
            c = 0;
            if (bakugan->stats != NULL) {
                c = BtlStatsGetCounter(bakugan->stats, 5);
            }
            if (a + b + c == 0) {
                fire = true;
            }
        }
    }
    if (fire) {
        msgId = BtlHudAdvicePickMessage(self, slot, false);
    }
    BtlHudAdviceStep(self, fire, msgId, slot);
}
