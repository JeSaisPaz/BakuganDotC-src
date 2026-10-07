// bdc 0x08838c88 BtlHudAdviceTrigger04
#include "bdc.h"

/* Advice slot 4 (only while `target` is non-NULL), driven by the unit's battle stats counter 0x13
   (`BtlStatsGetCounter`; 0 without a stats block). State 0 snapshots the counter into
   adviceAux and advances; state 1 advances once the counter differs from the snapshot; state 2
   picks the advisor message (`BtlHudAdviceGetFace`, `BtlHudAdvicePickMessage`) into adviceMsg
   and, once `UiTalkShowBattleMessage` shows it, re-arms to state 0. No advisor or no message
   disables the slot (state 9999). Negative states and states 3+ do nothing. */
void BtlHudAdviceTrigger04(BtlHud *self, BtlBakugan *unit, void *target, int slot)
{
    int counter;
    int face;
    s16 state;

    if (target == NULL) {
        return;
    }
    counter = 0;
    if (unit->stats != NULL) {
        counter = BtlStatsGetCounter(unit->stats, 0x13);
    }
    state = self->adviceState[slot];
    if (state <= 0) {
        if (state >= 0) {
            self->adviceAux[slot] = (s16)counter;
            self->adviceState[slot] = state + 1;
        }
    } else if (state < 2) {
        if (self->adviceAux[slot] != counter) {
            self->adviceState[slot] = state + 1;
        }
    } else if (state < 3) {
        face = BtlHudAdviceGetFace();
        if (face == -1) {
            self->adviceState[slot] = 9999;
            return;
        }
        self->adviceMsg[slot] = (s16)BtlHudAdvicePickMessage(self, slot, false);
        if (self->adviceMsg[slot] == -1) {
            self->adviceState[slot] = 9999;
        } else if (UiTalkShowBattleMessage(self, face, self->adviceMsg[slot], -1, 0) != 0) {
            self->adviceState[slot] = 0;
        }
    }
}
