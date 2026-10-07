// bdc 0x089a8230 UiMainMenuPhaseNetConnect
#include "bdc.h"

/* Phase 5 of `UiMainMenu` (entered from the battle-mode select's ad-hoc choice).
   Step 0 waits for task 1999 to end; step 1 polls the menu result: 0 returns to the battle-mode
   select (task 350, phase 3), 1/2 open the ad-hoc lobby task 2001. Step 2 waits for it, showing
   the connecting status from frame 150 and giving up at 900 (`NetAdhocBeginStop`, profile flag
   0x80); then shows message 14 when the connection failed (flag 0x80), or opens the battle-rule
   select (task 340, phase 4) with the net sprites/guest help refreshed. Step 3 waits for the
   message window and returns to the battle-mode select; step 4 slides the title back in; any
   other step returns to phase 2, step 6. */

void UiMainMenuPhaseNetConnect(UiMainMenu *self)
{
    u32 step;
    s32 result;
    s32 timer;
    bool failed;
    bool done;

    UiMainMenuBobModels(self);
    UiMainMenuRefreshFlagSprites(self);
    step = (u32)self->base.phaseStep;
    if (step >= 5) {
        UiMainMenuSetNetSpritesVisible(self, 1);
        self->base.phase = 2;
        self->base.phaseStep = 6;
        return;
    }
    switch (step) {
    case 1:
        result = UiGetMenuResult(&self->base);
        if (result > 0) {
            if (result < 3) {
                self->base.phaseStep = 2;
                CoreTaskCreate(2001, 100);
                self->netTimer = 0;
            }
        } else if (result >= 0) {
            g_uiKeepSharedBg = 1;
            CoreTaskCreate(350, 100);
            UiMainMenuSetNetSpritesVisible(self, 1);
            self->base.phase = 3;
            self->base.phaseStep = 2;
        }
        break;
    case 2:
        if (CoreTaskExists(2001) != 0) {
            timer = self->netTimer + 1;
            self->netTimer = timer;
            if (timer < 900) {
                if (timer >= 150) {
                    NetStatusSetMessage(1, 0);
                }
            } else {
                NetStatusSetMessage(0, 0);
                done = true;
                if (NetAdhocHasManager()) {
                    if (!NetAdhocBeginStop((NetAdhocConn *)NetAdhocGetManager())) {
                        done = false;
                    }
                }
                if (done) {
                    SaveProfileSetFlags(SaveGetProfile(), 0x80);
                }
            }
            break;
        }
        failed = false;
        if (SaveHasProfile()) {
            if (SaveProfileHasFlags(SaveGetProfile(), 0x80)) {
                failed = true;
            }
        }
        if (failed) {
            self->base.phaseStep = 3;
            if (!UiMsgWindowExists()) {
                UiMsgWindowEnsure();
            }
            ((UiMsgWindow *)UiMsgWindowGet())->mode = 1;
            UiMsgWindowOpen((UiMsgWindow *)UiMsgWindowGet(), 0, 14);
        } else {
            CoreTaskCreate(340, 100);
            self->base.phase = 4;
            self->base.phaseStep = 2;
            UiMainMenuRefreshNetSprites(self);
            UiMainMenuShowGuestHelp(self);
        }
        break;
    case 3:
        done = true;
        if (UiMsgWindowExists()) {
            done = false;
            if (UiMsgWindowIsClosed((UiMsgWindow *)UiMsgWindowGet())) {
                done = true;
            }
        }
        if (done) {
            g_uiKeepSharedBg = 1;
            CoreTaskCreate(350, 100);
            UiMainMenuSetNetSpritesVisible(self, 1);
            self->base.phase = 3;
            self->base.phaseStep = 2;
        }
        break;
    case 4:
        if (UiMainMenuStepTitleSlide(self, 0) == 1) {
            UiMainMenuSetBattleSpritesVisible(self, 1);
            UiMainMenuShowItemLabel(self, 1, (u8)self->cursor);
            self->base.phaseStep = 5;
        }
        break;
    default:
        if (CoreTaskExists(1999) == 0) {
            self->base.phaseStep = 1;
        }
        break;
    }
}
