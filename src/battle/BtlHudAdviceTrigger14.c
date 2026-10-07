// bdc 0x08839688 BtlHudAdviceTrigger14
#include "bdc.h"

/* Advice trigger of slot 14: while the slot has no message (`adviceMsg[slot] == -1`), once the
   unit's statistics counters 0+1+2 (`BtlStatsGetCounter`) sum to at least 20 it marks the
   slot (`adviceMsg[slot] = 0`) and fires when counters 3+4+5 are all zero. A unit without a
   statistics record counts 0 for every counter; the record pointer is re-read after each counter
   call, as in the original. A firing slot picks its message with `BtlHudAdvicePickMessage`
   (normal line); always ends with `BtlHudAdviceStep``(self, fire, msgId, slot)` (msgId -1 when
   not firing). */

void BtlHudAdviceTrigger14(BtlHud *self, void *unit, int slot)
{
    BtlBakugan *bakugan = unit;
    bool fire = false;
    int msgId = -1;
    BtlStats *stats;
    int c0, c1, c2;

    if (self->adviceMsg[slot] == -1) {
        stats = bakugan->stats;
        c0 = 0;
        if (stats != NULL) {
            c0 = BtlStatsGetCounter(stats, 0);
            stats = bakugan->stats;
        }
        c1 = 0;
        if (stats != NULL) {
            c1 = BtlStatsGetCounter(stats, 1);
            stats = bakugan->stats;
        }
        c2 = 0;
        if (stats != NULL) {
            c2 = BtlStatsGetCounter(stats, 2);
        }
        if (c0 + c1 + c2 >= 20) {
            self->adviceMsg[slot] = 0;
            stats = bakugan->stats;
            c0 = 0;
            if (stats != NULL) {
                c0 = BtlStatsGetCounter(stats, 3);
                stats = bakugan->stats;
            }
            c1 = 0;
            if (stats != NULL) {
                c1 = BtlStatsGetCounter(stats, 4);
                stats = bakugan->stats;
            }
            c2 = 0;
            if (stats != NULL) {
                c2 = BtlStatsGetCounter(stats, 5);
            }
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
