// bdc 0x088389f8 BtlHudAdviceTrigger02
#include "bdc.h"

/* Advice slot trigger 2, stepping `adviceState[slot]` only once slots 0 and 1 have messages
   (`adviceMsg[0]` and `adviceMsg[1]` != -1): step 0 advances once the unit is in state 4; step 1
   waits until it leaves state 4, then goes to step 10 when its HP ratio (`BtlCombatGetHpRatio`)
   is below 0.3 and back to step 0 otherwise; step 10 picks the advisor face
   (`BtlHudAdviceGetFace`) and a message (`BtlHudAdvicePickMessage`) other than slot 1's into
   `adviceMsg[slot]` and shows it (`UiTalkShowBattleMessage`), advancing to step 11 (idle) once
   shown. No face or no message parks the slot at 9999. */
void BtlHudAdviceTrigger02(BtlHud *self, BtlBakugan *unit, int slot)
{
    u32 faceId;
    s32 msg;

    if (self->adviceMsg[0] == -1 || self->adviceMsg[1] == -1) {
        return;
    }
    switch (self->adviceState[slot]) {
    case 0:
        if (unit->state == 4) {
            self->adviceState[slot] = self->adviceState[slot] + 1;
        }
        break;
    case 1:
        if (unit->state != 4) {
            self->adviceState[slot] =
                BtlCombatGetHpRatio(&unit->combat) < 0.300000012f ? 10 : 0;
        }
        break;
    case 10:
        faceId = BtlHudAdviceGetFace();
        if (faceId == 0xffffffffu) {
            self->adviceState[slot] = 9999;
            break;
        }
        do {
            msg = BtlHudAdvicePickMessage(self, slot, false);
        } while (self->adviceMsg[1] == msg);
        self->adviceMsg[slot] = (s16)msg;
        if (self->adviceMsg[slot] == -1) {
            self->adviceState[slot] = 9999;
        } else if (UiTalkShowBattleMessage(self, faceId, self->adviceMsg[slot], -1, 0) != 0) {
            self->adviceState[slot] = self->adviceState[slot] + 1;
        }
        break;
    default:
        break;
    }
}
