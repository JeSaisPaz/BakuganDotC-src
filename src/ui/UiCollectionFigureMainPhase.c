// bdc 0x089915d8 UiCollectionFigureMainPhase
#include "bdc.h"

/* Main phase (entry 2 of the phase table `0x08a9e9f0`) of the figure collection screen (task 314,
   `maybe_UiScreen314Ctor`; 3x2 grid pages of collected metal figures shown as 3D models
   (`"00_P_Dragonoid_N_U_figure.gmo"`…, environment map `"figure_refmap"`), names
   `"cha_spherename_colle_%02d"`, help `"DWCollectionHelp"`; cursor `+0xe78`, page `+0xe79`,
   category `+0xe7d`, entry lists `+0x11c0`): opens all parts, then handles cursor moves
   (`UiCollectionFigureMoveCursor`), page changes (`UiCollectionFigureChangePage`, sub-states 6..9 with
   `UiCollectionFigureAnimatePageArrow`), opening an entry (10..0xe: model to the centre, panels,
   help text `UiCollectionFigureSetHelpText`), the second detail view (0xf..0x13) and cancel (close → 0x14 → next
   phase). Every frame it then pulses and spins the cell models. */

static inline void PlaySe(u32 id)
{
  if (SndHasManager()) {
    SndManagerPlay(SndGetManager(), id, 0, 0);
  }
}

void UiCollectionFigureMainPhase(UiCollectionFigure *self)
{
  u8 done;
  u8 confirm;

  switch (self->base.phaseStep) {
  case 0:
    UiCollectionFigureStartBgTween(self, 0);
    UiCollectionFigureStartCellButtonTween(self, 0);
    UiCollectionFigureStartFrameTween(self, 0);
    UiCollectionFigureStartPageArrowTween(self, 0);
    UiCollectionFigureStartCellModels(self, 0);
    self->base.phaseStep++;
    break;
  case 1:
    done = UiCollectionFigureUpdateBgTween(self, 0);
    done += UiCollectionFigureCellButtonsDone(self, 0);
    done += UiCollectionFigureFrameDone(self, 0);
    done += UiCollectionFigurePageArrowsDone(self, 0);
    done += UiCollectionFigureUpdateCellModels(self, false);
    if (done == 5) {
      UiCollectionFigureSetModelPulse(self, 1);
      UiCollectionFigureSetModelSpin(self, 1);
      UiCollectionFigureResetCursor(self);
      self->base.phaseStep++;
    }
    break;
  case 2:
    UiCollectionFigurePulseCursor(self);
    UiCollectionFigurePulseSelectedCell(self);
    UiCollectionFigureZoomSelectedCell(self);
    /* tween slot 0x44 (+0xb14) holds the UiPulse of the cursor ghost sprite 0x44 */
    UiPulseStep(((GfxSprite **)self->base.data)[0x44], (UiPulse *)&self->tweens[0x44]);
    confirm = (u8)UiCollectionFigureCheckConfirm(self);
    if (confirm == 0) {
      if ((self->base.pad->pressed & 0x2000) != 0) {
        PlaySe(2);
        self->cancelled = 1;
        UiCollectionFigureHideCursor(self);
        UiCollectionFigureSetModelPulse(self, 0);
        UiCollectionFigureSetModelSpin(self, 0);
        self->base.phaseStep = 3;
      } else if (UiCollectionFigureMoveCursor(self) == 1) {
        PlaySe(1);
        UiCollectionFigureResetCursor(self);
      } else if (UiCollectionFigureChangePage(self) == 1) {
        PlaySe(1);
        UiCollectionFigureResetCursor(self);
        UiCollectionFigureHideCursor(self);
        UiCollectionFigureSetModelPulse(self, 0);
        UiCollectionFigureSetModelSpin(self, 0);
        self->base.phaseStep = 6;
      }
    } else if (confirm == 1) {
      PlaySe(0);
      UiCollectionFigureSetModelPulse(self, 0);
      UiCollectionFigureSetModelSpin(self, 0);
      UiCollectionFigureResetCursor(self);
      UiCollectionFigureHideCursor(self);
      UiCollectionFigureSetModelPulse(self, 0);
      UiCollectionFigureSetModelSpin(self, 0);
      UiCollectionFigureStartSelectFlash(self);
      self->cancelled = 0;
      self->base.phaseStep = 5;
    } else {
      PlaySe(3);
    }
    break;
  case 3:
    UiCollectionFigureStartBgTween(self, 1);
    UiCollectionFigureStartCellButtonTween(self, 1);
    UiCollectionFigureStartFrameTween(self, 1);
    UiCollectionFigureStartPageArrowTween(self, 1);
    UiCollectionFigureStartCellModels(self, 1);
    self->base.phaseStep++;
    break;
  case 4:
    done = UiCollectionFigureUpdateBgTween(self, 1);
    done += UiCollectionFigureCellButtonsDone(self, 1);
    done += UiCollectionFigureFrameDone(self, 1);
    done += UiCollectionFigurePageArrowsDone(self, 1);
    done += UiCollectionFigureUpdateCellModels(self, true);
    if (done == 5) {
      self->base.phaseStep = 0x14;
    }
    break;
  case 5:
    if (UiCollectionFigureSelectFlashDone(self) == 1) {
      self->base.phaseStep = 10;
    }
    break;
  case 6:
    UiCollectionFigureStartCellButtonTween(self, 1);
    UiCollectionFigureStartCellModels(self, 1);
    UiCollectionFigureStartPageArrowAnim(self);
    self->base.phaseStep++;
    break;
  case 7:
    UiCollectionFigureAnimatePageArrow(self);
    done = UiCollectionFigureCellButtonsDone(self, 1);
    done += UiCollectionFigureUpdateCellModels(self, true);
    if (done == 2) {
      self->page = self->targetPage;
      UiCollectionFigureSetPageNumber(self);
      UiCollectionFigureClampCursor(self);
      self->base.phaseStep++;
    }
    break;
  case 8:
    UiCollectionFigureAnimatePageArrow(self);
    UiCollectionFigureStartCellButtonTween(self, 0);
    UiCollectionFigureStartCellModels(self, 0);
    self->base.phaseStep++;
    break;
  case 9:
    done = (u8)UiCollectionFigureAnimatePageArrow(self);
    done += UiCollectionFigureCellButtonsDone(self, 0);
    done += UiCollectionFigureUpdateCellModels(self, false);
    if (done == 3) {
      self->pageDir = 0;
      UiCollectionFigureResetCursor(self);
      UiCollectionFigureSetModelPulse(self, 1);
      UiCollectionFigureSetModelSpin(self, 1);
      UiCollectionFigureSetPageArrowAlpha(self, 1.0f);
      self->base.phaseStep = 2;
    }
    break;
  case 10:
    UiCollectionFigureMarkDetailCell(self);
    UiCollectionFigureSetHelpText(self);
    UiCollectionFigureStartModelToDetail(self, false);
    UiCollectionFigureStartHelpTextFade(self, 0);
    UiCollectionFigureStartDimFade(self, 0, 0);
    UiCollectionFigureStartDetailPanelTween(self, 0);
    UiCollectionFigureStartDetailNameTween(self, 0);
    self->base.phaseStep++;
    break;
  case 11:
    done = UiCollectionFigureMoveModelToDetail(self, false);
    done += UiCollectionFigureHelpTextFadeDone(self, 0);
    done += UiCollectionFigureDimFadeDone(self, 0, 0);
    done += UiCollectionFigureDetailPanelDone(self, 0);
    done += UiCollectionFigureDetailNameDone(self, 0);
    if (done == 5) {
      UiCollectionFigureSetModelSpin(self, 1);
      UiCollectionFigureSetDetailIcon(self, 0);
      self->base.phaseStep++;
    }
    break;
  case 12:
    if ((self->base.pad->pressed & 0x4000) != 0) {
      PlaySe(0);
      UiCollectionFigureSetModelSpin(self, 0);
      self->base.phaseStep = 0xf;
    } else if ((self->base.pad->pressed & 0x2000) != 0) {
      PlaySe(2);
      UiCollectionFigureMarkEntrySeen(self);
      self->base.phaseStep = 0xd;
    }
    break;
  case 13:
    UiCollectionFigureStartModelToDetail(self, true);
    UiCollectionFigureStartHelpTextFade(self, 1);
    UiCollectionFigureStartDimFade(self, 1, 0);
    UiCollectionFigureStartDetailPanelTween(self, 1);
    UiCollectionFigureStartDetailNameTween(self, 1);
    self->base.phaseStep++;
    break;
  case 14:
    done = UiCollectionFigureMoveModelToDetail(self, true);
    done += UiCollectionFigureHelpTextFadeDone(self, 1);
    done += UiCollectionFigureDimFadeDone(self, 1, 0);
    done += UiCollectionFigureDetailPanelDone(self, 1);
    done += UiCollectionFigureDetailNameDone(self, 1);
    if (done == 5) {
      UiCollectionFigureUnmarkDetailCell(self);
      UiCollectionFigureResetCursor(self);
      UiCollectionFigureSetModelPulse(self, 1);
      UiCollectionFigureSetModelSpin(self, 1);
      UiCollectionFigureSetDetailIcon(self, 1);
      self->base.phaseStep = 2;
    }
    break;
  case 15:
    UiCollectionFigureResetDetailPose(self, false);
    UiCollectionFigureStartDimFade(self, 0, 1);
    UiCollectionFigureStartModelToAltDetail(self, false);
    UiCollectionFigureStartMotionButtonsTween(self, 0);
    self->base.phaseStep++;
    break;
  case 16:
    done = UiCollectionFigureDimFadeDone(self, 0, 1);
    done += UiCollectionFigureMoveModelToAltDetail(self, false);
    done += UiCollectionFigureMotionButtonsDone(self, 0);
    if (done == 3) {
      UiCollectionFigureSetButtonGuide(self, 1);
      self->base.phaseStep++;
    }
    break;
  case 17:
    UiCollectionFigureAutoRotateModel(self);
    UiCollectionFigureRotateModelByPad(self);
    UiCollectionFigureZoomModelByPad(self);
    UiCollectionFigureUpdateZoomIcons(self);
    UiCollectionFigureApplyModelPose(self);
    if ((self->base.pad->pressed & 0x2000) != 0) {
      PlaySe(2);
      self->base.phaseStep = 0x12;
    }
    break;
  case 18:
    UiCollectionFigureResetDetailPose(self, true);
    UiCollectionFigureStartDimFade(self, 1, 1);
    UiCollectionFigureStartModelToAltDetail(self, true);
    UiCollectionFigureStartMotionButtonsTween(self, 1);
    self->base.phaseStep++;
    break;
  case 19:
    done = UiCollectionFigureDimFadeDone(self, 1, 1);
    done += UiCollectionFigureMoveModelToAltDetail(self, true);
    done += UiCollectionFigureMotionButtonsDone(self, 1);
    if (done == 3) {
      UiCollectionFigureSetButtonGuide(self, 0);
      UiCollectionFigureSetModelSpin(self, 1);
      self->base.phaseStep = 0xc;
    }
    break;
  default:
    UiCollectionFigureSetResultNone(self);
    self->base.phaseStep = 0;
    self->base.phase++;
    break;
  }
  UiCollectionFigurePulseCellModels(self);
  UiCollectionFigureSpinSelectedModel(self);
}
