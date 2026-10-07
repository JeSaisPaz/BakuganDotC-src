// bdc 0x08837ee4 BtlHudAdviceStage36HintA
#include "bdc.h"

/* Stage-0x24 advice hint A on advice slot `slot` (step `adviceState[slot]`, timer
   `adviceMsg[slot]`): step 0 waits until script global variable 11 is at least 0xf and the unit's
   special-art ready counter (`combat.artReadyFrames`) is non-zero; step 1 shows talk message 0x39
   (voice 0x2a14, `UiTalkShowBattleMessage`) and, once shown, caption 10 (`UiCaptionSetText`
   with `g_btlHudCaptionEmpty`); step 2 shows message 0x3a (voice 0x2a15) when message 0x39 is
   the current one (else skips it) and resets the timer; step 3 hides the caption when the timer
   reaches 181; negative steps and steps from 4 count the timer and re-arm (step 0) once it reaches
   451. */

void BtlHudAdviceStage36HintA(BtlHud *self, BtlBakugan *unit, int slot)
{
    s16 step;
    bool shown;

    step = self->adviceState[slot];
    if (step >= 0 && step < 2) {
        if (step > 0) {
            if (UiTalkShowBattleMessage(self, 0, 0x39, 0x2a14, 0) != 0) {
                UiCaptionSetText(self, 1, 10, g_btlHudCaptionEmpty, -1);
                self->adviceState[slot] = self->adviceState[slot] + 1;
            }
        } else if (g_scriptGlobalVars[11] >= 0xf && unit->combat.artReadyFrames != 0) {
            self->adviceState[slot] = step + 1;
        }
        return;
    }
    if (step == 2) {
        shown = false;
        if (self->talkMsgId == 0x39) {
            if (UiTalkShowBattleMessage(self, 0, 0x3a, 0x2a15, 0) != 0) {
                shown = true;
            }
        } else {
            shown = true;
        }
        if (shown) {
            self->adviceMsg[slot] = 0;
            self->adviceState[slot] = self->adviceState[slot] + 1;
        }
        return;
    }
    if (step == 3) {
        self->adviceMsg[slot] = (s16)(self->adviceMsg[slot] + 1);
        if (self->adviceMsg[slot] >= 0xb5) {
            UiCaptionSetText(self, 0, 0, NULL, -1);
            self->adviceState[slot] = self->adviceState[slot] + 1;
        }
        return;
    }
    self->adviceMsg[slot] = (s16)(self->adviceMsg[slot] + 1);
    if (self->adviceMsg[slot] >= 0x1c3) {
        self->adviceState[slot] = 0;
    }
}
