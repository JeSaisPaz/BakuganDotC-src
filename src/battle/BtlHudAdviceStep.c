// bdc 0x08837dc0 BtlHudAdviceStep
#include "bdc.h"

/* Per-slot advice state machine shared by the `BtlHudUpdateAdvice` triggers, on
   `adviceState[slot]`: state 0 — when `fire` is set and `msgId != -1`, stores `msgId` in
   `adviceMsg[slot]` and moves to state 1; state 1 — gets the advisor face
   (`BtlHudAdviceGetFace`): none (-1) sets state 9999 (slot disabled); otherwise, when
   `adviceMsg[slot] != -1`, shows it (`UiTalkShowBattleMessage` with no forcing) and moves to
   state 2 once that returns nonzero. Negative states and states ≥ 2 do nothing. */
void BtlHudAdviceStep(BtlHud *self, bool fire, int msgId, int slot)
{
    s16 state;
    int faceId;

    state = self->adviceState[slot];
    if (state == 0) {
        if (fire && msgId != -1) {
            self->adviceMsg[slot] = (s16)msgId;
            self->adviceState[slot] = 1;
        }
    } else if (state == 1) {
        faceId = BtlHudAdviceGetFace();
        if (faceId == -1) {
            self->adviceState[slot] = 9999;
        } else if (self->adviceMsg[slot] != -1) {
            if (UiTalkShowBattleMessage(self, (u32)faceId, self->adviceMsg[slot], -1, 0) != 0) {
                self->adviceState[slot] = self->adviceState[slot] + 1;
            }
        }
    }
}
