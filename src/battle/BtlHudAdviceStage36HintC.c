// bdc 0x08838198 BtlHudAdviceStage36HintC
#include "bdc.h"

/* Stage-0x24 advice hint C on advice slot `slot` (step `adviceState[slot]`, timer
   `adviceMsg[slot]`): step 0 waits until script global variable 11 is 0xd and the unit's HP
   (`BtlCombatGetHp`) is below its max HP (`BtlCombatGetMaxHp`, converted as unsigned); step 1
   shows talk message 0x37 (voice 0x2a13, last argument = message 0x37 is the current one,
   `UiTalkShowBattleMessage`) and, once shown, caption 0xb (`UiCaptionSetText` with
   `g_btlHudCaptionEmpty`) and resets the timer; step 2 hides the caption when the timer reaches
   181; negative steps and steps from 3 count the timer and re-arm (step 0) once it reaches 601. */

void BtlHudAdviceStage36HintC(BtlHud *self, BtlBakugan *unit, int slot)
{
    s16 step;
    float hp;
    u32 maxHp;

    step = self->adviceState[slot];
    if (step == 0) {
        if (g_scriptGlobalVars[11] != 0xd) {
            return;
        }
        hp = BtlCombatGetHp(&unit->combat);
        maxHp = (u32)BtlCombatGetMaxHp(&unit->combat);
        if (hp < (float)maxHp) {
            self->adviceState[slot] = self->adviceState[slot] + 1;
        }
        return;
    }
    if (step == 1) {
        if (UiTalkShowBattleMessage(self, 0, 0x37, 0x2a13, self->talkMsgId == 0x37) != 0) {
            UiCaptionSetText(self, 1, 0xb, g_btlHudCaptionEmpty, -1);
            self->adviceMsg[slot] = 0;
            self->adviceState[slot] = self->adviceState[slot] + 1;
        }
        return;
    }
    if (step == 2) {
        self->adviceMsg[slot] = (s16)(self->adviceMsg[slot] + 1);
        if (self->adviceMsg[slot] >= 0xb5) {
            UiCaptionSetText(self, 0, 0, NULL, -1);
            self->adviceState[slot] = self->adviceState[slot] + 1;
        }
        return;
    }
    self->adviceMsg[slot] = (s16)(self->adviceMsg[slot] + 1);
    if (self->adviceMsg[slot] > 600) {
        self->adviceState[slot] = 0;
    }
}
