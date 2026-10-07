// bdc 0x08838088 BtlHudAdviceStage36HintB
#include "bdc.h"

/* Stage-0x24 advice hint B on advice slot `slot` (step `adviceState[slot]`): step 0 waits until
   script global variable 11 is 9 and the unit's `stage36HintFlag` is set; step 1 shows talk
   message 0x35 (`UiTalkShowBattleMessage`, face 0) and, once it was shown, the caption line
   (`UiCaptionSetText` with `g_btlHudCaptionEmpty`); step 2 hides the caption once variable 11
   is no longer 9. Each step advances `adviceState[slot]`; negative steps and steps above 2 do
   nothing. */

void BtlHudAdviceStage36HintB(BtlHud *self, BtlBakugan *unit, int slot)
{
    s16 step;

    step = self->adviceState[slot];
    if (step > 0) {
        if (step < 2) {
            if (UiTalkShowBattleMessage(self, 0, 0x35, 0x2a11, 0) != 0) {
                UiCaptionSetText(self, 1, 2, g_btlHudCaptionEmpty, -1);
                self->adviceState[slot] = self->adviceState[slot] + 1;
            }
        } else if (step < 3) {
            if (g_scriptGlobalVars[11] != 9) {
                UiCaptionSetText(self, 0, 0, NULL, -1);
                self->adviceState[slot] = self->adviceState[slot] + 1;
            }
        }
    } else if (step >= 0) {
        if (g_scriptGlobalVars[11] == 9 && unit->stage36HintFlag != 0) {
            self->adviceState[slot] = step + 1;
        }
    }
}
