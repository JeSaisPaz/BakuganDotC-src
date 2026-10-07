// bdc 0x0882fd04 BtlHudUpdateStageIntro
#include "bdc.h"

/* Stage-intro animation widget of the battle HUD (`BtlHudUpdate`) (`BtlHudPhaseMain`), driven
   by introStep when the .fab table exists (steps outside 0..2 do nothing).
   Step 0 waits until the battle task (task 100, `BtlGetCameraTask`) is not busy, script flag
   0x22 is set (`CoreBitsetTest`) and window 0xb is active; then, for a multi-round match
   (`BtlHudIsMultiRoundMatch`), plays announcer voice 0x29fa (multiRoundSkip set) or 0x29f6 +
   profile word 0x1e clamped to 0..4 and arms a 50-frame timer for the follow-up; otherwise it plays
   0x29fb at once. It then advances to step 1 in the same frame.
   Step 1 counts the timer down and at 0 plays the follow-up voice 0x29fb (skipped when it already
   played), then advances to step 2 in the same frame.
   Step 2 runs the first .fab (`GfxFabUpdate`) with sounds 0x200133 at timer 10 and 0x200134 at
   timer 28 (when a sound manager exists), and once its first clip's frame reaches the clip's last
   frame (0 without a definition, `GfxFabGetClip`) marks the intro done, unlocks the units
   (`BtlSetControlLockAll`), advances to step 3, activates window 0xc and sets script flag
   0x1f. */

void BtlHudUpdateStageIntro(BtlHud *self)
{
    s32 step;
    s32 roundVoice;
    void *fab;
    GfxFabClip *clip;
    u32 lastFrame;

    if (self->fabs == NULL) {
        return;
    }
    step = self->introStep;
    if (step < 0 || step > 2) {
        return;
    }
    if (step == 0) {
        if (BtlCameraTaskExists() != 0 && ((BtlMain *)BtlGetCameraTask())->busy != 0) {
            return;
        }
        if (!CoreBitsetTest(0x22, g_scriptGlobalBits)) {
            return;
        }
        if (UiGetWindowActive(0xb) != 1) {
            return;
        }
        if (BtlHudIsMultiRoundMatch(self) != 0) {
            roundVoice = (s32)SaveProfileGetWord(SaveGetProfile(), 0x1e);
            if (roundVoice < 0) {
                roundVoice = 0;
            } else if (roundVoice > 4) {
                roundVoice = 4;
            }
            if (self->multiRoundSkip != 0) {
                SndBgmPlayVoice(0x29fa);
            } else {
                SndBgmPlayVoice(roundVoice + 0x29f6);
            }
            self->introTimer = 50;
            self->introVoiceDone = 0;
        } else {
            SndBgmPlayVoice(0x29fb);
            self->introVoiceDone = 1;
            self->introTimer = 0;
        }
        self->introStep = self->introStep + 1;
    }
    if (step <= 1) {
        if (self->introVoiceDone == 0) {
            self->introTimer = self->introTimer - 1;
            if (self->introTimer > 0) {
                return;
            }
            SndBgmPlayVoice(0x29fb);
            self->introVoiceDone = 1;
            self->introTimer = 0;
        }
        self->introStep = self->introStep + 1;
    }
    fab = self->fabs[0];
    if (fab == NULL) {
        return;
    }
    lastFrame = 0;
    GfxFabUpdate(fab);
    self->introTimer = self->introTimer + 1;
    if (self->introTimer == 10) {
        if (SndHasManager()) {
            SndManagerPlay(SndGetManager(), 0x200133, 0, 0);
        }
    } else if (self->introTimer == 28) {
        if (SndHasManager()) {
            SndManagerPlay(SndGetManager(), 0x200134, 0, 0);
        }
    }
    clip = (GfxFabClip *)GfxFabGetClip(self->fabs[0], 0);
    if (clip == NULL) {
        return;
    }
    if (clip->def != NULL) {
        lastFrame = clip->def->lastFrame;
    }
    if (clip->frame < lastFrame) {
        return;
    }
    self->stageIntroDone = 1;
    BtlSetControlLockAll(0);
    self->introStep = self->introStep + 1;
    UiSetWindowActive(0xc, 1);
    CoreBitsetSet(0x1f, g_scriptGlobalBits);
}
