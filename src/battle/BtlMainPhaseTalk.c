// bdc 0x0884ee14 BtlMainPhaseTalk
#include "bdc.h"

/* Phase 4 of the battle main task, a state machine on `talkStep` (`+0x480`): step 1 clears
   `dimColor` and starts the battle talk of `talkUnit` (`UiTalkStartBattleTalk` on the
   `BtlHud` from `UiGetTalkTask`); step 2 waits for the HUD's `cutInState` to drop to 0, then
   sets step 100. Steps 11–13 set `dimColor` to 1, hide window 0 and the cut-in sprites, free the
   cut-in textures and fade `dimColor[3]` by 0.8 per frame (0 at or below 0.005) until task 0x1e0
   is gone. Steps 3–9, any step outside 1..13, and the end of step 13 clear `dimColor`, return
   to phase/draw phase 1 (2 once `g_btlBattleOutcome` is set), resume the stage event script
   and reactivate window 0. */

void BtlMainPhaseTalk(BtlMain *self)
{
    float alpha;

    switch (self->talkStep) {
    case 1:
        self->dimColor[0] = 0.0f;
        self->dimColor[1] = 0.0f;
        self->dimColor[2] = 0.0f;
        self->dimColor[3] = 0.0f;
        UiTalkStartBattleTalk(UiGetTalkTask(), self->talkUnit);
        self->talkStep = self->talkStep + 1;
        /* fallthrough */
    case 2:
        if (UiGetTalkTask()->cutInState != 0) {
            return;
        }
        self->talkStep = 10;
        self->talkStep = 100;
        return;
    case 10:
        self->talkStep = 100;
        return;
    case 11:
        self->dimColor[0] = 1.0f;
        self->dimColor[1] = 1.0f;
        self->dimColor[2] = 1.0f;
        self->dimColor[3] = 1.0f;
        UiSetWindowActive(0, 0);
        UiTalkHideCutInSprites(UiGetTalkTask());
        self->talkStep = self->talkStep + 1;
        return;
    case 12:
        UiTalkFreeCutInTextures(UiGetTalkTask());
        self->talkStep = self->talkStep + 1;
        /* fallthrough */
    case 13:
        alpha = self->dimColor[3];
        if (!(alpha <= 0.005f)) {
            alpha = alpha * 0.8f;
        } else {
            alpha = 0.0f;
        }
        self->dimColor[3] = alpha;
        if (CoreTaskExists(0x1e0) != 0) {
            return;
        }
        self->talkStep = 100;
        /* fallthrough */
    default:
        self->dimColor[0] = 0.0f;
        self->dimColor[1] = 0.0f;
        self->dimColor[2] = 0.0f;
        self->dimColor[3] = 0.0f;
        if (g_btlBattleOutcome != 0) {
            self->phase = 2;
            self->drawPhase = 2;
        } else {
            self->phase = 1;
            self->drawPhase = 1;
        }
        BtlStageResumeEventScript();
        UiSetWindowActive(0, 1);
        return;
    }
}
