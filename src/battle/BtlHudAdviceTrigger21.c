// bdc 0x08839dfc BtlHudAdviceTrigger21
#include "bdc.h"

/* Advice slot 21, a state machine on `adviceState[slot]`: state 0 waits for the unit's
   `adviceTimer` to be -1; state 1 shows the advisor line (`BtlHudAdviceGetFace`,
   `BtlHudAdvicePickMessage`, `UiTalkShowBattleMessage`; no advisor → state 9999) and, once
   shown, sets the unit's `adviceFlag`, advances to state 2 and opens button-guide page 8
   (`BtlHudRequestButtonGuide`); state 2 waits for `adviceTimer` to leave -1, then returns to
   state 0, clears `talkMsgId` to -1 and closes the button guide. */

void BtlHudAdviceTrigger21(BtlHud *self, BtlBakugan *unit, int slot)
{
    s16 state = self->adviceState[slot];
    int face;

    if (state <= 0) {
        if (state >= 0 && unit->adviceTimer == -1) {
            self->adviceState[slot] = 1;
        }
    } else if (state < 2) {
        face = BtlHudAdviceGetFace();
        if (face == -1) {
            self->adviceState[slot] = 9999;
        } else if (UiTalkShowBattleMessage(self, face, BtlHudAdvicePickMessage(self, slot, false),
                                           -1, 0) != 0) {
            unit->adviceFlag = 1;
            self->adviceState[slot] = self->adviceState[slot] + 1;
            BtlHudRequestButtonGuide(self, true, 8);
        }
    } else if (state < 3 && unit->adviceTimer != -1) {
        self->adviceState[slot] = 0;
        self->talkMsgId = -1;
        BtlHudRequestButtonGuide(self, false, 0);
    }
}
