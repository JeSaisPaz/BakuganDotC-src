// bdc 0x089816e8 UiCollectionSphereMainPhase
#include "bdc.h"

/* Main phase (entry 2 of the phase table `0x08a9dc88`) of the sphere (Bakugan figure) collection
   screen (task 312, `maybe_UiScreen312Ctor`; 3x2 grid pages of collected Bakugan shown as 3D
   models (`"00_P_Dragonoid_N_P.gmo"`…), names `"cha_spherename_colle_%02d"`, pop-out motions
   (`"00_dor_dir_popout"`…); cursor `+0xee0`, page `+0xee1`, category `+0xee5`, entry lists
   `+0x1250`): opens all parts, then handles cursor moves (`UiCollectionSphereMoveCursor`), page
   changes (`UiCollectionSphereChangePage`, sub-states 6..9), opening an entry (sub-states
   10..0xe: detail view with the model moved to the centre and the help text), the motion view
   (0xf..0x13) and cancel (`cancelled` = 1, close → 0x14 → next phase). Ends every frame with
   `UiCollectionSphereUpdateModelGlow` and `UiCollectionSphereSpinPreviewModel`. */

void UiCollectionSphereMainPhase(UiCollectionSphere *self)
{
  u8 done;
  u8 confirm;
  PadState *pad;

  switch (self->base.phaseStep) {
  case 0:
    UiCollectionSphereStartBgTween(self, 0);
    UiCollectionSphereStartCellButtonTween(self, 0);
    UiCollectionSphereStartFrameTween(self, 0);
    UiCollectionSphereStartArrowTween(self, 0);
    UiCollectionSphereStartCellModels(self, 0);
    self->base.phaseStep++;
    break;
  case 1:
    done = UiCollectionSphereUpdateBgTween(self, 0);
    done += UiCollectionSphereUpdateCellButtonTween(self, 0);
    done += UiCollectionSphereUpdateFrameTween(self, 0);
    done += UiCollectionSphereUpdateArrowTween(self, 0);
    done += UiCollectionSphereUpdateCellModels(self, false);
    if (done == 5) {
      UiCollectionSphereResetCursor(self);
      UiCollectionSphereEnableModelGlow(self, 1);
      UiCollectionSphereEnablePreview(self, 1);
      self->base.phaseStep = 2;
    }
    break;
  case 2:
    UiCollectionSpherePulseCursor(self);
    UiCollectionSphereAnimateCell(self);
    UiCollectionSphereZoomCursorCell(self);
    UiPulseStep(((GfxSprite **)self->base.data)[0x46], (UiPulse *)&self->tweens[0x46]);
    confirm = (u8)UiCollectionSphereCheckConfirm(self);
    if (confirm == 0) {
      if ((self->base.pad->pressed & 0x2000) != 0) {
        /* cancel: close the screen */
        if (SndHasManager()) {
          SndManagerPlay(SndGetManager(), 2, 0, 0);
        }
        self->cancelled = 1;
        UiCollectionSphereHideCursor(self);
        UiCollectionSphereEnableModelGlow(self, 0);
        UiCollectionSphereEnablePreview(self, 0);
        self->base.phaseStep = 3;
      } else if (UiCollectionSphereMoveCursor(self) == 1) {
        if (SndHasManager()) {
          SndManagerPlay(SndGetManager(), 1, 0, 0);
        }
        UiCollectionSphereResetCursor(self);
      } else if (UiCollectionSphereChangePage(self) == 1) {
        if (SndHasManager()) {
          SndManagerPlay(SndGetManager(), 1, 0, 0);
        }
        UiCollectionSphereResetCursor(self);
        UiCollectionSphereHideCursor(self);
        UiCollectionSphereEnableModelGlow(self, 0);
        UiCollectionSphereEnablePreview(self, 0);
        self->base.phaseStep = 6;
      }
    } else if (confirm == 1) {
      if (SndHasManager()) {
        SndManagerPlay(SndGetManager(), 0, 0, 0);
      }
      UiCollectionSphereEnableModelGlow(self, 0);
      UiCollectionSphereEnablePreview(self, 0);
      UiCollectionSphereResetCursor(self);
      UiCollectionSphereHideCursor(self);
      UiCollectionSphereEnableModelGlow(self, 0);
      UiCollectionSphereEnablePreview(self, 0);
      UiCollectionSphereStartCellPress(self);
      self->cancelled = 0;
      self->base.phaseStep = 5;
    } else {
      if (SndHasManager()) {
        SndManagerPlay(SndGetManager(), 3, 0, 0);
      }
    }
    break;
  case 3:
    UiCollectionSphereStartBgTween(self, 1);
    UiCollectionSphereStartCellButtonTween(self, 1);
    UiCollectionSphereStartFrameTween(self, 1);
    UiCollectionSphereStartArrowTween(self, 1);
    UiCollectionSphereStartCellModels(self, 1);
    self->base.phaseStep++;
    break;
  case 4:
    done = UiCollectionSphereUpdateBgTween(self, 1);
    done += UiCollectionSphereUpdateCellButtonTween(self, 1);
    done += UiCollectionSphereUpdateFrameTween(self, 1);
    done += UiCollectionSphereUpdateArrowTween(self, 1);
    done += UiCollectionSphereUpdateCellModels(self, true);
    if (done == 5) {
      self->base.phaseStep = 0x14;
    }
    break;
  case 5:
    if (UiCollectionSphereWaitPress(self) == 1) {
      self->base.phaseStep = 10;
    }
    break;
  case 6:
    UiCollectionSphereStartCellButtonTween(self, 1);
    UiCollectionSphereStartCellModels(self, 1);
    UiCollectionSphereStartPageArrowPress(self);
    self->base.phaseStep++;
    break;
  case 7:
    UiCollectionSphereAnimatePageArrow(self);
    done = UiCollectionSphereUpdateCellButtonTween(self, 1);
    done += UiCollectionSphereUpdateCellModels(self, true);
    if (done == 2) {
      self->page = self->oldPage;
      UiCollectionSphereSetPageNumber(self);
      UiCollectionSphereClampCursorToPage(self);
      UiCollectionSphereRepositionCameras(self);
      self->base.phaseStep++;
    }
    break;
  case 8:
    UiCollectionSphereAnimatePageArrow(self);
    UiCollectionSphereStartCellButtonTween(self, 0);
    UiCollectionSphereStartCellModels(self, 0);
    self->base.phaseStep++;
    break;
  case 9:
    done = (u8)UiCollectionSphereAnimatePageArrow(self);
    done += UiCollectionSphereUpdateCellButtonTween(self, 0);
    done += UiCollectionSphereUpdateCellModels(self, false);
    if (done == 3) {
      self->pageDir = 0;
      UiCollectionSphereResetCursor(self);
      UiCollectionSphereEnableModelGlow(self, 1);
      UiCollectionSphereEnablePreview(self, 1);
      UiCollectionSphereDimPageArrows(1.0f, &self->base);
      self->base.phaseStep = 2;
    }
    break;
  case 10:
    UiCollectionSphereSelectDetailModel(self);
    UiCollectionSphereSetHelpText(self);
    UiCollectionSphereStartModelToDetail(self, false);
    UiCollectionSphereStartDim(self, 0, 0);
    UiCollectionSphereStartDetailPanelTween(self, 0);
    UiCollectionSphereStartDetailNameTween(self, 0);
    UiCollectionSphereStartHelpFade(self, 0);
    self->base.phaseStep++;
    break;
  case 0xb:
    done = UiCollectionSphereMoveModelToDetail(self, false);
    done += UiCollectionSphereUpdateDim(self, 0, 0);
    done += UiCollectionSphereUpdateDetailPanelTween(self, 0);
    done += UiCollectionSphereUpdateDetailNameTween(self, 0);
    done += UiCollectionSphereUpdateHelpFade(self, 0);
    if (done == 5) {
      UiCollectionSphereEnablePreview(self, 1);
      UiCollectionSphereSetHelpIcon(&self->base, 0);
      self->base.phaseStep++;
    }
    break;
  case 0xc:
    pad = self->base.pad;
    if ((pad->pressed & 0x4000) != 0) {
      /* open the motion view */
      if (SndHasManager()) {
        SndManagerPlay(SndGetManager(), 0, 0, 0);
      }
      UiCollectionSphereEnablePreview(self, 0);
      self->base.phaseStep = 0xf;
    } else if ((pad->pressed & 0x2000) != 0) {
      /* back to the grid */
      if (SndHasManager()) {
        SndManagerPlay(SndGetManager(), 2, 0, 0);
      }
      UiCollectionSphereMarkEntrySeen(self);
      self->base.phaseStep = 0xd;
    }
    break;
  case 0xd:
    UiCollectionSphereStartModelToDetail(self, true);
    UiCollectionSphereStartDim(self, 1, 0);
    UiCollectionSphereStartDetailPanelTween(self, 1);
    UiCollectionSphereStartDetailNameTween(self, 1);
    UiCollectionSphereStartHelpFade(self, 1);
    self->base.phaseStep++;
    break;
  case 0xe:
    done = UiCollectionSphereMoveModelToDetail(self, true);
    done += UiCollectionSphereUpdateDim(self, 1, 0);
    done += UiCollectionSphereUpdateDetailPanelTween(self, 1);
    done += UiCollectionSphereUpdateDetailNameTween(self, 1);
    done += UiCollectionSphereUpdateHelpFade(self, 1);
    if (done == 5) {
      UiCollectionSphereClearDetailModel(self);
      UiCollectionSphereResetCursor(self);
      UiCollectionSphereEnableModelGlow(self, 1);
      UiCollectionSphereEnablePreview(self, 1);
      UiCollectionSphereSetHelpIcon(&self->base, 1);
      self->base.phaseStep = 2;
    }
    break;
  case 0xf:
    UiCollectionSphereBuildMotionButtonMask(self);
    UiCollectionSphereResetDetailPose(self, false);
    UiCollectionSphereStartDim(self, 0, 1);
    UiCollectionSphereStartModelToAltDetail(self, false);
    UiCollectionSphereStartMotionButtonsTween(self, 0);
    self->base.phaseStep++;
    break;
  case 0x10:
    done = UiCollectionSphereUpdateDim(self, 0, 1);
    done += UiCollectionSphereMoveModelToAltDetail(self, false);
    done += UiCollectionSphereUpdateMotionButtonsTween(self, 0);
    if (done == 3) {
      UiCollectionSphereShowBgForMotionView(self, 1);
      if (UiCollectionSphereGetPageKind(self, (u8)self->page) != 2) {
        if (SndHasManager()) {
          SndManagerPlay(SndGetManager(), 0xb, 0, 0);
        }
      }
      UiCollectionSphereLoadPopoutMotion(self, (u8)self->detailCell);
      self->base.phaseStep++;
    }
    break;
  case 0x11:
    UiCollectionSphereUpdatePopoutMotion(self, (u8)self->detailCell);
    UiCollectionSphereAutoRotate(self);
    UiCollectionSphereRotateDetailModel(self);
    UiCollectionSphereZoomDetailModel(self);
    UiCollectionSphereDimZoomIcons(self);
    UiCollectionSpherePlayPopout(self);
    UiCollectionSphereApplyDetailPose(self);
    if ((self->base.pad->pressed & 0x2000) != 0) {
      if (SndHasManager()) {
        SndManagerPlay(SndGetManager(), 2, 0, 0);
      }
      UiCollectionSphereRewindPopoutMotion(self, (u8)self->detailCell);
      self->base.phaseStep = 0x12;
    }
    break;
  case 0x12:
    UiCollectionSphereResetDetailPose(self, true);
    UiCollectionSphereStartDim(self, 1, 1);
    UiCollectionSphereStartModelToAltDetail(self, true);
    UiCollectionSphereStartMotionButtonsTween(self, 1);
    self->base.phaseStep++;
    break;
  case 0x13:
    done = UiCollectionSphereUpdateDim(self, 1, 1);
    done += UiCollectionSphereMoveModelToAltDetail(self, true);
    done += UiCollectionSphereUpdateMotionButtonsTween(self, 1);
    if (done == 3) {
      UiCollectionSphereShowBgForMotionView(self, 0);
      UiCollectionSphereEnablePreview(self, 1);
      self->base.phaseStep = 0xc;
    }
    break;
  default:
    UiCollectionSphereSetResultNone(self);
    self->base.phaseStep = 0;
    self->base.phase++;
    break;
  }
  UiCollectionSphereUpdateModelGlow(self);
  UiCollectionSphereSpinPreviewModel(self);
}
