// bdc 0x0893d114 UiUnlockResultMainPhase
#include "bdc.h"

/* Main phase (entry 2 of the phase table `0x08a9c800`) of the unlock-result screen (task 375,
   `UiUnlockResultCtor`): plays the open animation of all parts, waits for the reveal flash and
   the confirm button, plays the close animation, then applies the reward and switches the screen to
   its exit phase. */

void UiUnlockResultMainPhase(UiUnlockResult *self)
{
  u8 done;
  s32 flashDone;

  switch ((u32)self->base.phaseStep) {
  case 0:
    if (self->loadTimer != 0) {
      self->loadTimer = self->loadTimer - 1;
    } else {
      self->base.phaseStep = self->base.phaseStep + 1;
    }
    break;
  case 1:
    if (SndHasManager()) {
      SndManagerPlay(SndGetManager(), 0x2c0000a, 0, 0);
    }
    UiUnlockResultStartPanelTween(self, false);
    UiUnlockResultStartRewardPop(self, 0);
    UiUnlockResultStartHologramIconPop(self, 0);
    UiUnlockResultStartHologramSprite07Pop(self, 0);
    UiUnlockResultShowPieces(self, 0);
    UiUnlockResultStartSprite05Pop(self, 0);
    UiUnlockResultStartNamePlatePop(self, 0);
    UiUnlockResultStartNameTextFade(self, 0);
    UiUnlockResultStartCardTypePop(self, 0);
    UiUnlockResultStartArenaPhotoPop(self, 0);
    UiUnlockResultSetRewardName(self);
    UiUnlockResultSetHelpText(self);
    UiUnlockResultAttachFrameEdges(self);
    UiUnlockResultLayoutFrame(self);
    self->base.phaseStep = self->base.phaseStep + 1;
    break;
  case 2:
    done = UiUnlockResultUpdatePanelTween(self, false);
    done += UiUnlockResultRewardPopDone(self, 0);
    done += UiUnlockResultHologramIconPopDone(self, 0);
    done += UiUnlockResultHologramSprite07PopDone(self, 0);
    done += UiUnlockResultPiecesDone(self, 0);
    done += UiUnlockResultSprite05PopDone(self, 0);
    done += UiUnlockResultNamePlatePopDone(self, 0);
    done += UiUnlockResultNameTextFadeDone(self, 0);
    done += UiUnlockResultCardTypePopDone(self, 0);
    done += UiUnlockResultArenaPhotoPopDone(self, 0);
    UiUnlockResultAttachFrameEdges(self);
    UiUnlockResultLayoutFrame(self);
    UiUnlockResultResetFlash(self);
    if (done == 10) {
      UiUnlockResultShowButtonPrompt(self, 1);
      UiUnlockResultShowItemBoxCounter(self, true);
      UiUnlockResultPlayModelMotion(self);
      UiUnlockResultSetHelpTextVisible(self, 1);
      UiUnlockResultResetBlink(self);
      self->base.phaseStep = self->base.phaseStep + 1;
    }
    break;
  case 3:
    UiUnlockResultUpdateModel(self);
    UiUnlockResultBlinkSprite05(self);
    flashDone = UiUnlockResultAnimateFlash(self);
    if ((self->base.pad->pressed & 0x4000) != 0 && flashDone != 0) {
      if (SndHasManager()) {
        SndManagerPlay(SndGetManager(), 0, 0, 0);
      }
      UiUnlockResultShowButtonPrompt(self, 0);
      UiUnlockResultShowItemBoxCounter(self, false);
      self->base.phaseStep = self->base.phaseStep + 1;
    }
    break;
  case 4:
    UiUnlockResultSetHelpTextVisible(self, 0);
    UiUnlockResultStartPanelTween(self, true);
    UiUnlockResultStartRewardPop(self, 1);
    UiUnlockResultStartHologramIconPop(self, 1);
    UiUnlockResultStartHologramSprite07Pop(self, 1);
    UiUnlockResultShowPieces(self, 1);
    UiUnlockResultStartSprite05Pop(self, 1);
    UiUnlockResultStartNamePlatePop(self, 1);
    UiUnlockResultStartNameTextFade(self, 1);
    UiUnlockResultStartCardTypePop(self, 1);
    UiUnlockResultStartArenaPhotoPop(self, 1);
    UiUnlockResultAttachFrameEdges(self);
    UiUnlockResultLayoutFrame(self);
    self->base.phaseStep = self->base.phaseStep + 1;
    break;
  case 5:
    done = UiUnlockResultUpdatePanelTween(self, true);
    done += UiUnlockResultRewardPopDone(self, 1);
    done += UiUnlockResultHologramIconPopDone(self, 1);
    done += UiUnlockResultHologramSprite07PopDone(self, 1);
    done += UiUnlockResultPiecesDone(self, 1);
    done += UiUnlockResultSprite05PopDone(self, 1);
    done += UiUnlockResultNamePlatePopDone(self, 1);
    done += UiUnlockResultNameTextFadeDone(self, 1);
    done += UiUnlockResultCardTypePopDone(self, 1);
    done += UiUnlockResultArenaPhotoPopDone(self, 1);
    UiUnlockResultAttachFrameEdges(self);
    UiUnlockResultLayoutFrame(self);
    if (done == 10) {
      self->base.phaseStep = 6;
    }
    break;
  default:
    /* phaseStep >= 6 (unsigned compare: negative values land here too) */
    UiUnlockResultApplyReward(self);
    UiUnlockResultSetResult(self);
    self->base.phase = 3;
    self->base.phaseStep = 0;
    break;
  }
}
