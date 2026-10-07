// bdc 0x08839b98 BtlHudAdviceTrigger18
#include "bdc.h"

/* Advice trigger (slot 18 when called by `BtlHudUpdateAdvice`): does nothing when `target` is NULL.
   Otherwise fires when slots 16 and 17 already hold a message (`adviceMsg[0x10]` and `adviceMsg[0x11]`
   not -1), no message is pending in `slot` (`adviceMsg[slot] == -1`) and the sum of the target's
   statistics counters 0xf, 0xa and 0xb (`BtlStatsGetCounter` on `target->stats`; each read as 0
   without a stats record) is at least 15; a fired trigger picks the normal line for `slot`
   (`BtlHudAdvicePickMessage`). Then steps the slot's advice state machine (`BtlHudAdviceStep`).
   `unit` is unused. */

void BtlHudAdviceTrigger18(BtlHud *self, void *unit, BtlBakugan *target, int slot)
{
    bool fire;
    int msgId;
    BtlStats *stats;
    int counterF;
    int counterA;
    int counterB;

    (void)unit;
    if (target == NULL) {
        return;
    }
    fire = false;
    if (self->adviceMsg[0x10] != -1 && self->adviceMsg[0x11] != -1 && self->adviceMsg[slot] == -1) {
        stats = target->stats;
        counterF = 0;
        if (stats != NULL) {
            counterF = BtlStatsGetCounter(stats, 0xf);
            stats = target->stats;
        }
        counterA = 0;
        if (stats != NULL) {
            counterA = BtlStatsGetCounter(stats, 0xa);
            stats = target->stats;
        }
        counterB = 0;
        if (stats != NULL) {
            counterB = BtlStatsGetCounter(stats, 0xb);
        }
        if (counterF + counterA + counterB >= 15) {
            fire = true;
        }
    }
    msgId = -1;
    if (fire) {
        msgId = BtlHudAdvicePickMessage(self, slot, false);
    }
    BtlHudAdviceStep(self, fire, msgId, slot);
}
