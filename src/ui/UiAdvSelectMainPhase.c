// bdc 0x0891b434 UiAdvSelectMainPhase
#include "bdc.h"

/* Main phase of the adventure partner-select screen: sub-state machine on `phaseStep` that slides
   the frame and panels in, runs the intro help, handles cursor movement and the decide/locked input,
   the confirm help and the decide sequence, then slides everything out, fades the screen to black and
   leaves for phase 3. An out-of-range step clears the result, closes the shared background, clears
   g_uiKeepSharedBg and switches to phase 3. Every frame ends by updating the blinks, the arrow flash
   and the name-panel tweens. */

void UiAdvSelectMainPhase(UiAdvSelect *self)
{
  u8 running;
  u8 decide;
  GfxFader *fader;

  switch (self->base.phaseStep) {
  case 0:
    UiAdvSelectTweenFrame(self, 0);
    self->base.phaseStep = self->base.phaseStep + 1;
    break;
  case 1:
    if ((u8)UiAdvSelectFrameDone(self, 0) == 1) {
      UiAdvSelectLoadPedestal(self);
      self->base.phaseStep = self->base.phaseStep + 1;
    }
    break;
  case 2:
    UiAdvSelectTweenPartnerPicture(self, 0);
    UiAdvSelectTweenArrows(self, 0);
    UiAdvSelectTweenHelpPanel(self, 0);
    UiAdvSelectTweenNamePanels(self, 1, 0);
    UiAdvSelectTweenCandidates(self, 0);
    UiAdvSelectLoadBakuganModel(self);
    self->base.phaseStep = self->base.phaseStep + 1;
    break;
  case 3:
    running = (u8)UiAdvSelectPartnerPictureDone(self, 0);
    running = (u8)(running + UiAdvSelectArrowsDone(self, 0));
    running = (u8)(running + UiAdvSelectHelpPanelDone(self, 0));
    if ((u8)(running + UiAdvSelectCandidatesDone(self, 0)) == 4) {
      UiAdvSelectStartBlinkA(self, 1);
      UiAdvSelectStartBlinkB(self, 1);
      UiAdvSelectStartArrowFlash(self, 1);
      UiAdvSelectStartHelp(self, 0);
      self->base.phaseStep = self->base.phaseStep + 1;
    }
    break;
  case 4:
    if (UiAdvSelectRunHelp(self) == 1) {
      UiAdvSelectUpdateCursor(self);
      self->base.phaseStep = self->base.phaseStep + 1;
    }
    break;
  case 5:
    UiAdvSelectStartCursorPulse(self);
    UiAdvSelectAnimateFocus(self);
    UiPulseStep(((GfxSprite **)self->base.data)[0x27], (UiPulse *)&self->tweens[0x27]);
    decide = (u8)UiAdvSelectCheckDecide(self);
    if (decide == 0) {
      if (UiAdvSelectMoveCursorPrev(self) == 1) {
        if (SndHasManager()) {
          SndManagerPlay(SndGetManager(), 1, 0, 0);
        }
        UiAdvSelectUpdateCursor(self);
        UiAdvSelectClearSelection(self);
        self->base.phaseStep = 6;
      }
    }
    else if (decide == 1) {
      if (SndHasManager()) {
        SndManagerPlay(SndGetManager(), 0, 0, 0);
      }
      UiAdvSelectUpdateCursor(self);
      UiAdvSelectFlashSelection(self);
      self->flag898 = 0;
      self->base.phaseStep = 13;
    }
    else {
      /* locked candidate: error sound only */
      if (SndHasManager()) {
        SndManagerPlay(SndGetManager(), 3, 0, 0);
      }
    }
    break;
  case 6:
    UiAdvSelectLoadBakuganModel(self);
    self->base.phaseStep = 5;
    break;
  case 7:
    UiAdvSelectHideCursors(self);
    UiAdvSelectTweenPartnerPicture(self, 1);
    UiAdvSelectTweenArrows(self, 1);
    UiAdvSelectTweenHelpPanel(self, 1);
    UiAdvSelectTweenNamePanels(self, 1, 1);
    UiAdvSelectTweenCandidates(self, 1);
    UiAdvSelectStartBlinkA(self, 0);
    UiAdvSelectStartBlinkB(self, 0);
    UiAdvSelectStartArrowFlash(self, 0);
    self->base.phaseStep = self->base.phaseStep + 1;
    break;
  case 8:
    running = (u8)UiAdvSelectPartnerPictureDone(self, 1);
    running = (u8)(running + UiAdvSelectArrowsDone(self, 1));
    running = (u8)(running + UiAdvSelectHelpPanelDone(self, 1));
    if ((u8)(running + UiAdvSelectCandidatesDone(self, 1)) == 4) {
      UiAdvSelectReleaseModel(self);
      UiAdvSelectReleasePedestal(self);
      self->base.phaseStep = self->base.phaseStep + 1;
    }
    break;
  case 9:
    UiAdvSelectTweenFrame(self, 1);
    self->base.phaseStep = self->base.phaseStep + 1;
    break;
  case 10:
    if ((u8)UiAdvSelectFrameDone(self, 1) == 1) {
      self->base.phaseStep = self->base.phaseStep + 1;
    }
    break;
  case 11:
    SndBgmCancelChannel(0);
    SndBgmQueueStop(0.1f, 0);
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
    self->base.phaseStep = self->base.phaseStep + 1;
    break;
  case 12:
    if (GfxFaderIsFinished(GfxGetActiveFader())) {
      self->base.phaseStep = 16;
    }
    break;
  case 13:
    if (UiAdvSelectFlashDone() == 1) {
      UiAdvSelectStartHelp(self, 1);
      self->base.phaseStep = 14;
    }
    break;
  case 14:
    if (UiAdvSelectRunHelp(self) == 1) {
      if (self->helpCancelled == 0) {
        UiAdvSelectResetDecide(self);
        self->base.phaseStep = self->base.phaseStep + 1;
      }
      else {
        self->base.phaseStep = 5;
      }
    }
    break;
  case 15:
    if (UiAdvSelectRunDecide(self) == 1) {
      UiAdvSelectMarkChosen(self);
      self->base.phaseStep = 7;
    }
    break;
  default:
    UiAdvSelectSetResultNone(self);
    UiSharedBgClose();
    g_uiKeepSharedBg = 0;
    self->base.phase = 3;
    self->base.phaseStep = 0;
    break;
  }
  UiAdvSelectUpdateBlinkA(self);
  UiAdvSelectUpdateBlinkB(self);
  UiAdvSelectUpdateArrowFlash(self);
  UiAdvSelectNamePanelsDone(self);
  return;
}
