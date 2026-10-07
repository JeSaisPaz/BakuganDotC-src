// bdc 0x08838740 BtlHudAdviceTrigger00
#include "bdc.h"

/* Advice slot trigger 0, stepping `adviceState[slot]`: step 0 advances once the unit is in state
   4; step 1 waits until it leaves state 4, then goes to step 10 when its HP ratio
   (`BtlCombatGetHpRatio`) is below 0.9 and back to step 0 otherwise; step 10 picks the advisor
   face (`BtlHudAdviceGetFace`) and message (`BtlHudAdvicePickMessage` into `adviceMsg[slot]`)
   and shows it (`UiTalkShowBattleMessage`), advancing to step 11 (idle) once shown. No face or
   no message parks the slot at 9999. */
void BtlHudAdviceTrigger00(BtlHud *self, BtlBakugan *unit, int slot)
{
    u32 faceId;

    switch (self->adviceState[slot]) {
    case 0:
        if (unit->state == 4) {
            self->adviceState[slot] = 1;
        }
        break;
    case 1:
        if (unit->state != 4) {
            self->adviceState[slot] =
                BtlCombatGetHpRatio(&unit->combat) < 0.899999976f ? 10 : 0;
        }
        break;
    case 10:
        faceId = BtlHudAdviceGetFace();
        if (faceId == 0xffffffffu) {
            self->adviceState[slot] = 9999;
        } else {
            self->adviceMsg[slot] = (s16)BtlHudAdvicePickMessage(self, slot, false);
            if (self->adviceMsg[slot] == -1) {
                self->adviceState[slot] = 9999;
            } else if (UiTalkShowBattleMessage(self, faceId, self->adviceMsg[slot], -1, 0) != 0) {
                self->adviceState[slot] = self->adviceState[slot] + 1;
            }
        }
        break;
    default:
        break;
    }
}
