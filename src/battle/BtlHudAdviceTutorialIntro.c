// bdc 0x08837cb8 BtlHudAdviceTutorialIntro
#include "bdc.h"

/* Advice slot 0x16 (battle id 4). Does nothing without an advisor (`BtlHudAdviceGetFace` -1).
   State 0: advances once an attribute landmark is damaged
   (`ActorStageObjAnyAttrLandmarkDamaged`). State 1: advances once advisor message 0x2d4 is
   shown (`UiTalkShowBattleMessage`). State 2: if the current talk message is 0x2d4, advances
   once message 0x2d5 is shown; otherwise advances directly. Negative states and states 3+ do
   nothing. */
void BtlHudAdviceTutorialIntro(BtlHud *self, int slot)
{
    int face;
    s16 state;

    face = BtlHudAdviceGetFace();
    if (face == -1) {
        return;
    }
    state = self->adviceState[slot];
    if (state <= 0) {
        if (state < 0) {
            return;
        }
        if (ActorStageObjAnyAttrLandmarkDamaged() != 0) {
            self->adviceState[slot] = self->adviceState[slot] + 1;
        }
    } else if (state < 2) {
        if (UiTalkShowBattleMessage(self, face, 0x2d4, -1, 0) != 0) {
            self->adviceState[slot] = self->adviceState[slot] + 1;
        }
    } else if (state < 3) {
        if (self->talkMsgId != 0x2d4) {
            self->adviceState[slot] = state + 1;
        } else if (UiTalkShowBattleMessage(self, face, 0x2d5, -1, 0) != 0) {
            self->adviceState[slot] = self->adviceState[slot] + 1;
        }
    }
}
