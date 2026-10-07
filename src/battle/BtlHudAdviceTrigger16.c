// bdc 0x0883997c BtlHudAdviceTrigger16
#include "bdc.h"

/* Advice trigger (slot 16 when called by `BtlHudUpdateAdvice`): does nothing when `target` is NULL.
   Otherwise fires when no message is pending in `slot` (`adviceMsg[slot] == -1`) and the sum of the
   target's statistics counters 0xf, 0xa and 0xb (`BtlStatsGetCounter` on `target->stats`; each read
   as 0 without a stats record) is at least 5; a fired trigger picks the normal line for `slot`
   (`BtlHudAdvicePickMessage`). Then steps the slot's advice state machine (`BtlHudAdviceStep`).
   `unit` is unused. */

void BtlHudAdviceTrigger16(BtlHud *self, void *unit, BtlBakugan *target, int slot)
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
    msgId = -1;
    if (self->adviceMsg[slot] == -1) {
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
        if (counterF + counterA + counterB >= 5) {
            fire = true;
        }
    }
    if (fire) {
        msgId = BtlHudAdvicePickMessage(self, slot, false);
    }
    BtlHudAdviceStep(self, fire, msgId, slot);
}
