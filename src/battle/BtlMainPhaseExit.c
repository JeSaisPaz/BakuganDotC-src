// bdc 0x0884ea70 BtlMainPhaseExit
#include "bdc.h"

/* Phase-3 update of the battle main task (task id 100, `BtlMainTaskCtor`): the pause/exit
   state machine on `phaseStep`. Step 0, once the pause menu task (0x19a) is gone, acts on script
   variable 3: 0xf stores the event flags when the save snapshot is on
   (`SaveProfileStoreEventFlags`), sets profile flag 0x40000000, creates the save task 0x2724 and
   goes to step 10; 0xe creates screen task 0x12d and goes to step 1. Otherwise it un-pauses tasks
   0x6e, 0x1e0 and 0x1e1 (`CoreTaskClearFlags` 3), sets `phase`/`drawPhase` to 1 and step 0, and
   then: 1 nothing more; 2 resets the match (`BtlMainResetMatchState` with `keepRounds` cleared)
   and leaves to phase 2 step 0x14; 3..6, 0xb, 0x10 leave to phase 2 step 100; both of these
   suspend the event script and stop the BGM and voices; any other value resumes the event script
   unless the talk window holds it (`+0x919`). Step 1 reopens the pause menu once task 0x12d is
   gone. Step 10, once the save task is gone, clears profile flag 0x40000000, reopens the pause
   menu on its quit entry (`UiPauseSelectQuitEntry`) and, when the active fader is visible,
   fades it to transparent over 10 frames; then step 0. */

void BtlMainPhaseExit(BtlMain *self)
{
    void *pause;
    BtlHud *talk;
    GfxFader *fader;
    float *from;
    bool resume;

    if (self->phaseStep >= 2) {
        if (self->phaseStep != 10 || CoreTaskExists(0x2724) != 0) {
            return;
        }
        SaveProfileClearFlags(SaveGetProfile(), 0x40000000);
        BtlMainOpenPauseMenu();
        pause = CoreTaskFind(0x19a);
        if (pause != NULL) {
            UiPauseSelectQuitEntry(pause);
        }
        if (GfxGetActiveFader()->color[3] <= 0.0f) {
            self->phaseStep = 0;
            return;
        }
        fader = GfxGetActiveFader();
        from = GfxGetActiveFader()->color;
        fader->start[0] = from[0];
        fader->start[1] = from[1];
        fader->start[2] = from[2];
        fader->start[3] = from[3];
        fader = GfxGetActiveFader();
        fader->end[0] = 0.0f;
        fader->end[1] = 0.0f;
        fader->end[2] = 0.0f;
        fader->end[3] = 0.0f;
        GfxFaderStart(GfxGetActiveFader(), 10);
        self->phaseStep = 0;
        return;
    }
    if (self->phaseStep < 0) {
        return;
    }
    if (self->phaseStep >= 1) {
        if (CoreTaskExists(0x12d) == 0) {
            BtlMainOpenPauseMenu();
            self->phaseStep = 0;
        }
        return;
    }
    if (CoreTaskExists(0x19a) != 0) {
        return;
    }
    if (g_scriptGlobalVars[3] == 0xf) {
        if (g_saveSnapshotEnabled != 0) {
            SaveProfileStoreEventFlags();
        }
        SaveProfileSetFlags(SaveGetProfile(), 0x40000000);
        CoreTaskCreate(0x2724, 100);
        self->phaseStep = 10;
        return;
    }
    if (g_scriptGlobalVars[3] == 0xe) {
        CoreTaskCreate(0x12d, 100);
        self->phaseStep = self->phaseStep + 1;
        return;
    }
    if (CoreTaskExists(0x6e) != 0) {
        CoreTaskClearFlags(CoreTaskFind(0x6e), 3);
    }
    if (CoreTaskExists(0x1e0) != 0) {
        CoreTaskClearFlags(CoreTaskFind(0x1e0), 3);
    }
    if (CoreTaskExists(0x1e1) != 0) {
        CoreTaskClearFlags(CoreTaskFind(0x1e1), 3);
    }
    self->phase = 1;
    self->drawPhase = 1;
    self->phaseStep = 0;
    switch (g_scriptGlobalVars[3]) {
    case 1:
        break;
    case 2:
        self->keepRounds = 0;
        BtlMainResetMatchState(self);
        BtlStageSuspendEventScript();
        self->phase = 2;
        self->drawPhase = 2;
        self->phaseStep = 0x14;
        BtlStopBgm();
        SndManagerFadeOutAllVoices(SndGetManager());
        break;
    case 3:
    case 4:
    case 5:
    case 6:
    case 0xb:
    case 0x10:
        BtlStageSuspendEventScript();
        self->phase = 2;
        self->drawPhase = 2;
        self->phaseStep = 100;
        BtlStopBgm();
        SndManagerFadeOutAllVoices(SndGetManager());
        break;
    default:
        resume = true;
        if (UiTalkTaskExists() != 0) {
            talk = UiGetTalkTask();
            if (talk->talkScriptSuspended != 0) {
                resume = false;
            }
        }
        if (resume) {
            BtlStageResumeEventScript();
        }
        self->phase = 1;
        self->drawPhase = 1;
        self->phaseStep = 0;
        break;
    }
}
