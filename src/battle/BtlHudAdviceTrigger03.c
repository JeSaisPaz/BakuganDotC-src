// bdc 0x08838b64 BtlHudAdviceTrigger03
#include "bdc.h"

/* Advice slot trigger 3 (only while `target` is non-NULL), stepping `adviceState[slot]`: the
   unit's stats counter 0x12 (`BtlStatsGetCounter`, 0 without stats) is read every call. Step 0
   records it in `adviceAux[slot]`; step 1 advances once the counter differs from the record;
   step 2 picks the advisor face (`BtlHudAdviceGetFace`) and message
   (`BtlHudAdvicePickMessage` into `adviceMsg[slot]`) and shows it
   (`UiTalkShowBattleMessage`), re-arming to step 0 once shown. No face or no message parks the
   slot at 9999. */

void BtlHudAdviceTrigger03(BtlHud *self, BtlBakugan *unit, void *target, int slot)
{
    s16 step;
    int counter;
    u32 faceId;

    if (target == NULL) {
        return;
    }
    counter = 0;
    if (unit->stats != NULL) {
        counter = BtlStatsGetCounter(unit->stats, 0x12);
    }
    step = self->adviceState[slot];
    if (step < 1) {
        if (step >= 0) {
            self->adviceAux[slot] = (s16)counter;
            self->adviceState[slot] = step + 1;
        }
    } else if (step < 2) {
        if (self->adviceAux[slot] != counter) {
            self->adviceState[slot] = step + 1;
        }
    } else if (step < 3) {
        faceId = BtlHudAdviceGetFace();
        if (faceId == 0xffffffffu) {
            self->adviceState[slot] = 9999;
        } else {
            self->adviceMsg[slot] = (s16)BtlHudAdvicePickMessage(self, slot, false);
            if (self->adviceMsg[slot] == -1) {
                self->adviceState[slot] = 9999;
            } else if (UiTalkShowBattleMessage(self, faceId, self->adviceMsg[slot], -1, 0) != 0) {
                self->adviceState[slot] = 0;
            }
        }
    }
}
