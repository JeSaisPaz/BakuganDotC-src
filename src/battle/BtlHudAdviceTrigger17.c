// bdc 0x08839a84 BtlHudAdviceTrigger17
#include "bdc.h"

/* Advice trigger of slot 17: does nothing without a target. Otherwise fires once slot 16 has been
   checked (`adviceMsg[16] != -1`), this slot has no message, and the target's statistics counters
   0xf + 0xa + 0xb (`BtlStatsGetCounter`, 0 each without a record) sum to more than 9. A firing
   trigger picks its message with `BtlHudAdvicePickMessage` (main line); then
   `BtlHudAdviceStep``(self, fire, msgId, slot)` (msgId -1 when not firing). `unit` is unused. */

void BtlHudAdviceTrigger17(BtlHud *self, void *unit, void *target, int slot)
{
    BtlBakugan *targetUnit = target;
    bool fire;
    int msgId;
    int countF;
    int countA;
    int countB;

    (void)unit;
    if (targetUnit == NULL) {
        return;
    }
    fire = false;
    msgId = -1;
    if (self->adviceMsg[0x10] != -1 && self->adviceMsg[slot] == -1) {
        countF = 0;
        if (targetUnit->stats != NULL) {
            countF = BtlStatsGetCounter(targetUnit->stats, 0xf);
        }
        countA = 0;
        if (targetUnit->stats != NULL) {
            countA = BtlStatsGetCounter(targetUnit->stats, 0xa);
        }
        countB = 0;
        if (targetUnit->stats != NULL) {
            countB = BtlStatsGetCounter(targetUnit->stats, 0xb);
        }
        if (countF + countA + countB >= 10) {
            fire = true;
        }
    }
    if (fire) {
        msgId = BtlHudAdvicePickMessage(self, slot, false);
    }
    BtlHudAdviceStep(self, fire, msgId, slot);
}
