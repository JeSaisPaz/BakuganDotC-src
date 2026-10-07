// bdc 0x0893768c UiGauntletSetupMainPhase
#include "bdc.h"

/* Main phase of UiGauntletSetup: sub-state machine on phaseStep. Steps 0-3 open the panels and
   load the models (first visit: help dialog in steps 12-13), step 4 is the interactive loop
   (confirm / cancel / cursor), 11 finishes a confirm flash, 5-8 close the screen, 9-10 fade to
   black, and 14+ calls UiGauntletSetupFinish and advances the screen phase. Every frame the cell
   blink, scroll loop, slot decorations, card info panel and avatar animation are updated. */

void UiGauntletSetupMainPhase(UiGauntletSetup *self)
{
  GfxSprite **spr;
  GfxFader *fader;
  SaveProfile *profile;
  u8 done;
  u8 confirm;

  if ((u32)self->base.phaseStep < 14) {
    switch (self->base.phaseStep) {
    case 0:
      spr = (GfxSprite **)self->base.data;
      UiCellBlinkInit(1, spr[3], &self->cellBlink);
      spr = (GfxSprite **)self->base.data;
      UiScrollLoopInit(1, spr[4], spr[5], &self->scrollLoop);
      UiGauntletSetupTweenHeader(self, 0);
      self->base.phaseStep++;
      break;
    case 1:
      if ((u8)UiGauntletSetupFrameDone(self, 0) == 1) {
        self->base.phaseStep++;
      }
      break;
    case 2:
      UiGauntletSetupTweenFrame(self, 0);
      UiGauntletSetupTweenCardList(self, 0);
      UiGauntletSetupTweenOkButton(self, 0);
      UiGauntletSetupTweenCardSlots(self, 0);
      UiGauntletSetupSetCardInfoMode(self, 1);
      UiGauntletSetupLoadBakuganModel(self);
      UiGauntletSetupLoadPedestalModel(self);
      UiGauntletSetupLoadPlayerModel(self);
      self->base.phaseStep++;
      break;
    case 3:
      done = UiGauntletSetupTweenFrameDone(self, 0);
      done = (u8)(done + UiGauntletSetupCardListDone(self, 0));
      done = (u8)(done + UiGauntletSetupOkButtonDone(self, 0));
      done = (u8)(done + UiGauntletSetupSlotsDone(self, 0));
      if (done == 4) {
        profile = SaveGetProfile();
        if ((profile->data->viewSeenMask & 0x20) == 0) {
          self->base.phaseStep = 12;
        } else {
          UiGauntletSetupUpdateCursor(self);
          self->base.phaseStep++;
        }
      }
      break;
    case 4:
      UiGauntletSetupPulseCursor(self);
      UiGauntletSetupPulseOkButton(self);
      UiGauntletSetupAnimateFocus(self);
      spr = (GfxSprite **)self->base.data;
      UiPulseStep(spr[59], (UiPulse *)&self->tweens[59]);
      confirm = (u8)UiGauntletSetupHandleConfirm(self);
      if (confirm == 0) {
        if ((self->base.pad->pressed & 0x2000) != 0) {
          if (SndHasManager()) {
            SndManagerPlay(SndGetManager(), 2, 0, 0);
          }
          self->cancelled = 1;
          UiGauntletSetupUpdateCursor(self);
          self->base.phaseStep = 5;
        } else if (UiGauntletSetupMoveCursor(self) == 1) {
          if (SndHasManager()) {
            SndManagerPlay(SndGetManager(), 1, 0, 0);
          }
          UiGauntletSetupUpdateCursor(self);
        }
      } else if (confirm == 1) {
        if (SndHasManager()) {
          SndManagerPlay(SndGetManager(), 0, 0, 0);
        }
        UiGauntletSetupUpdateCursor(self);
        UiGauntletSetupStartConfirmFlash(self);
        self->cancelled = 0;
        self->base.phaseStep = 11;
      } else {
        if (SndHasManager()) {
          SndManagerPlay(SndGetManager(), 3, 0, 0);
        }
      }
      break;
    case 5:
      UiGauntletSetupHideCursor(self);
      UiGauntletSetupTweenFrame(self, 1);
      UiGauntletSetupTweenCardList(self, 1);
      UiGauntletSetupTweenOkButton(self, 1);
      UiGauntletSetupTweenCardSlots(self, 1);
      UiGauntletSetupSetCardInfoMode(self, 2);
      self->base.phaseStep++;
      break;
    case 6:
      done = UiGauntletSetupTweenFrameDone(self, 1);
      done = (u8)(done + UiGauntletSetupCardListDone(self, 1));
      done = (u8)(done + UiGauntletSetupOkButtonDone(self, 1));
      done = (u8)(done + UiGauntletSetupSlotsDone(self, 1));
      if (done == 4) {
        self->base.phaseStep++;
      }
      break;
    case 7:
      UiGauntletSetupTweenHeader(self, 1);
      UiGauntletSetupReleaseBakuganModel(self);
      UiGauntletSetupReleasePedestalModel(self);
      UiGauntletSetupReleasePlayerModel(self);
      self->base.phaseStep++;
      break;
    case 8:
      if ((u8)UiGauntletSetupFrameDone(self, 1) == 1) {
        self->base.phaseStep++;
      }
      break;
    case 9:
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
      GfxFaderStart(GfxGetActiveFader(), 16);
      self->base.phaseStep++;
      break;
    case 10:
      if (GfxFaderIsFinished(GfxGetActiveFader())) {
        self->base.phaseStep = 14;
      }
      break;
    case 11:
      if (UiGauntletSetupConfirmFlashDone(self) == 1) {
        if (self->focusArea == 0) {
          UiGauntletSetupRefreshSlotSprites(self);
          UiGauntletSetupSaveSlots(self);
          self->base.phaseStep = 4;
        } else {
          self->base.phaseStep = 5;
        }
      }
      break;
    case 12:
      self->helpStep = 0;
      self->base.phaseStep = 13;
      break;
    case 13:
      if (UiGauntletSetupShowFirstTimeHelp(self) == 1) {
        UiGauntletSetupUpdateCursor(self);
        profile = SaveGetProfile();
        profile->data->viewSeenMask |= 0x20;
        self->base.phaseStep = 4;
      }
      break;
    }
  } else {
    UiGauntletSetupFinish(self);
    self->base.phaseStep = 0;
    self->base.phase++;
  }
  UiCellBlinkUpdate(&self->cellBlink);
  UiScrollLoopUpdate(&self->scrollLoop);
  UiGauntletSetupAttachSlotSprites(self);
  UiGauntletSetupUpdateCardInfo(self);
  UiGauntletSetupUpdateAvatarAnim(self);
}
