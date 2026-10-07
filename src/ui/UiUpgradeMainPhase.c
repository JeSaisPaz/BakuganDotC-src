// bdc 0x08916cb4 UiUpgradeMainPhase
#include "bdc.h"

/* Main phase of the Bakugan upgrade screen, driven by `phaseStep`:
   0 starts the cell blink (sprite 4), the background scroll (sprites 2, 3) and the base-panel
   open tweens; 1 waits for them, then loads the Bakugan model and opens the other panels; 2 waits
   for those and goes to 100 (first-visit help prompt) unless save `viewSeenMask` bit 0x100 is set;
   3 refreshes cursor/message/colours; 4 handles input (Cross on the confirm button: result 1 and
   close; Cross on an affordable slot: step 200; Circle: result 0 and close; up/down move `focus`
   in 0..6 with wrap-around); 5..6 close the panels while fading `textColor` to 0; 7 starts a
   10-frame fade to black and frees the model; 8 waits for the fade and advances `phase`.
   100/101 run the help prompt (sets `viewSeenMask` bit 0x100), 200/201 the purchase confirmation
   (applying the purchase and going to 202 on yes), 202 pays the cost and returns to 3.
   Every frame it also steps the blink/scroll records, restarts the model's `motionName` motion
   once the `modelName` motion has ended, and steps the background fab until its last frame. */

void UiUpgradeMainPhase(UiUpgrade *self)
{
    s32 oldFocus;
    bool doneA;
    bool doneB;
    bool doneC;
    bool doneD;
    s32 next;
    s32 i;
    GfxModel *model;
    s32 motion;
    u32 frame;

    switch (self->base.phaseStep) {
    case 0:
        UiCellBlinkInit(1, ((GfxSprite **)self->base.data)[4], &self->cellBlink);
        UiScrollLoopInit(1, ((GfxSprite **)self->base.data)[2], ((GfxSprite **)self->base.data)[3],
                         &self->scrollLoop);
        UiUpgradeTweenBasePanels(self, 0);
        self->base.phaseStep = self->base.phaseStep + 1;
        break;
    case 1:
        if (UiUpgradeBasePanelsDone(self, 0)) {
            UiUpgradeRefreshSlotColors(self);
            UiUpgradeLoadBakuganModel(self);
            UiUpgradeTweenSlotPanel(self, 0);
            UiUpgradeTweenNamePanel(self, 0);
            UiUpgradeTweenSlotGrid(self, 0);
            UiUpgradeTweenHeaderPanels(self, 0);
            self->base.phaseStep = self->base.phaseStep + 1;
        }
        break;
    case 2:
        doneA = UiUpgradeSlotPanelDone(self, 0);
        doneB = UiUpgradeConfirmPanelsDone(self, 0);
        doneC = UiUpgradeDetailPanelsDone(self, 0);
        doneD = UiUpgradeHeaderPanelsDone(self, 0);
        if ((u8)(doneA + doneB + doneC + doneD) == 4) {
            next = 100;
            if ((SaveGetProfile()->data->viewSeenMask & 0x100) != 0) {
                next = self->base.phaseStep + 1;
            }
            self->base.phaseStep = next;
        }
        break;
    case 3:
        UiUpgradeUpdateCursor(self);
        UiUpgradeShowMessage(self, UiUpgradeGetUpgradeId(self->bakugan, self->focus));
        UiUpgradeRefreshSlotColors(self);
        self->base.phaseStep = self->base.phaseStep + 1;
        break;
    case 4:
        oldFocus = self->focus;
        if ((self->base.pad->pressed & 0x4000) != 0) {
            if (self->focus == 6) {
                if (SndHasManager()) {
                    SndManagerPlay(SndGetManager(), 0, 0, 0);
                }
                UiSetMenuResult(&self->base, 1);
                self->base.phaseStep = self->base.phaseStep + 1;
            } else if (UiUpgradeCanAfford(self, self->focus) != 0) {
                if (SndHasManager()) {
                    SndManagerPlay(SndGetManager(), 0, 0, 0);
                }
                self->base.phaseStep = 200;
            } else {
                if (SndHasManager()) {
                    SndManagerPlay(SndGetManager(), 3, 0, 0);
                }
            }
        } else if ((self->base.pad->pressed & 0x1000) != 0) {
            /* nothing */
        } else if ((self->base.pad->pressed & 0x2000) != 0) {
            if (SndHasManager()) {
                SndManagerPlay(SndGetManager(), 2, 0, 0);
            }
            UiSetMenuResult(&self->base, 0);
            self->base.phaseStep = self->base.phaseStep + 1;
        } else if ((self->base.pad->repeat & 0x10) != 0) {
            self->focus = self->focus - 1;
            if (self->focus < 0) {
                self->focus = 6;
            }
        } else if ((self->base.pad->repeat & 0x40) != 0) {
            self->focus = self->focus + 1;
            if (self->focus >= 7) {
                self->focus = 0;
            }
        }
        if (oldFocus != self->focus) {
            if (SndHasManager()) {
                SndManagerPlay(SndGetManager(), 1, 0, 0);
            }
            UiUpgradeRefreshSlotColors(self);
            UiUpgradeUpdateCursor(self);
            UiUpgradeShowMessage(self, UiUpgradeGetUpgradeId(self->bakugan, self->focus));
        }
        UiUpgradeStartCursorPulse(self);
        UiUpgradeStopSlotPulse(self);
        UiPulseStep(((GfxSprite **)self->base.data)[102], (UiPulse *)&self->tweens[0x66]);
        break;
    case 5:
        UiUpgradeTweenSlotPanel(self, 1);
        UiUpgradeTweenNamePanel(self, 1);
        UiUpgradeTweenSlotGrid(self, 1);
        UiUpgradeTweenHeaderPanels(self, 1);
        UiUpgradeHideCursors(self);
        self->base.phaseStep = self->base.phaseStep + 1;
        break;
    case 6:
        doneA = UiUpgradeSlotPanelDone(self, 1);
        doneB = UiUpgradeConfirmPanelsDone(self, 1);
        doneC = UiUpgradeDetailPanelsDone(self, 1);
        doneD = UiUpgradeHeaderPanelsDone(self, 1);
        /* textColor += (black - textColor) * 0.2 (VFPU vsub/vscl/vadd per lane) */
        for (i = 0; i < 4; i++) {
            self->textColor[i] = self->textColor[i] + (0.0f - self->textColor[i]) * 0.2f;
        }
        if ((u8)(doneA + doneB + doneC + doneD) == 4) {
            self->base.phaseStep = self->base.phaseStep + 1;
        }
        break;
    case 7: {
        GfxFader *fader;

        fader = GfxGetActiveFader();
        fader->start[0] = 0.0f;
        fader->start[1] = 0.0f;
        fader->start[2] = 0.0f;
        fader->start[3] = 0.0f;
        fader = GfxGetActiveFader();
        fader->end[0] = 0.0f;
        fader->end[1] = 0.0f;
        fader->end[2] = 0.0f;
        fader->end[3] = 1.0f;
        GfxFaderStart(GfxGetActiveFader(), 10);
        UiUpgradeTweenBasePanels(self, 1);
        CoreObjectDeferDelete((CoreObject *)self->model, 0);
        next = self->base.phaseStep;
        self->model = NULL;
        self->base.phaseStep = next + 1;
        break;
    }
    case 8:
        UiUpgradeBasePanelsDone(self, 1);
        if (GfxFaderIsFinished(GfxGetActiveFader())) {
            self->base.phaseStep = 0;
            self->base.phase = self->base.phase + 1;
        }
        break;
    case 100:
    case 200:
        UiUpgradeResetConfirm(self);
        self->base.phaseStep = self->base.phaseStep + 1;
        break;
    case 101:
        if (UiUpgradeRunConfirm(self, 0) != 0) {
            SaveGetProfile()->data->viewSeenMask |= 0x100;
            self->base.phaseStep = 3;
        }
        break;
    case 201:
        if (UiUpgradeRunConfirm(self, 1) != 0) {
            if (self->confirmChoice != 0) {
                UiUpgradeApplyPurchase(self, self->focus);
                self->base.phaseStep = 202;
            } else {
                self->base.phaseStep = 3;
            }
        }
        break;
    case 202:
        if (UiUpgradePayCost(self) != 0) {
            self->base.phaseStep = 3;
        }
        break;
    default:
        break;
    }

    UiCellBlinkUpdate(&self->cellBlink);
    UiScrollLoopUpdate(&self->scrollLoop);
    if (self->model != NULL) {
        model = (GfxModel *)self->model;
        motion = GmoMotionIndexOfName(GmoMotionMgrGet(), self->modelName);
        if (GfxModelIsMotion(model, motion) && ((GfxModel *)self->model)->motionEnded != 0) {
            model = (GfxModel *)self->model;
            motion = GmoMotionIndexOfName(GmoMotionMgrGet(), self->motionName);
            GfxModelPlayMotion(0.2f, model, motion, 1);
        }
    }
    if (self->base.bgData != NULL) {
        for (i = 0; i < 1; i++) {
            if (((GfxFab **)self->base.bgData)[i] != NULL) {
                frame = GfxFabGetFrame(((GfxFab **)self->base.bgData)[i]);
                if (frame != GfxFabGetFrameCount(((GfxFab **)self->base.bgData)[i])) {
                    GfxFabUpdate(((GfxFab **)self->base.bgData)[i]);
                }
            }
        }
    }
}
