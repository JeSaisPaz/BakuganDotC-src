// bdc 0x0892802c UiHologramGalleryMainPhase
#include "bdc.h"

/* Phase 2 of the hologram gallery screen (`UiHologramGalleryCtor`, task 391): the main sub-state
   machine on the step `+0x2c` — tweens in the base, menu, board, info/count/help panels; moves
   the cursor (`UiHologramGalleryMoveCursor`) and handles the decision
   (`UiHologramGalleryCheckDecide`, command menu `UiHologramGalleryDecideMenu`); browses the
   slot pages and the hologram list; places (`UiHologramGalleryPlaceHologram`, step 0x1e) or
   removes (`UiHologramGalleryRemoveHologram`, step 0x11) holograms; runs the help pages/dialog
   and bonus count; plays the menu sounds; opening the sub-screen moves to phase 3, closing to phase
   4 (with BGM stop and fade-out). Every frame it then steps the cell blink, the scroll loop, the
   slot/list pulses and the scroll arrows. */

#define SPRITE(i) (((GfxSprite **)self->base.data)[i])

#define PlayMenuSound(soundId)                          \
  do {                                                  \
    if (SndHasManager()) {                              \
      SndManagerPlay(SndGetManager(), (soundId), 0, 0); \
    }                                                   \
  } while (0)

void UiHologramGalleryMainPhase(UiHologramGallery *self)
{
  u8 done;
  u8 decide;
  GfxFader *fader;

  switch (self->base.phaseStep) {
  case 0:
    UiCellBlinkInit(1, SPRITE(4), &self->cellBlink);
    UiScrollLoopInit(1, SPRITE(2), SPRITE(3), &self->scrollLoop);
    UiHologramGalleryTweenBase(self, 0);
    UiHologramGalleryTweenMenu(self, 0);
    UiHologramGalleryTweenInfoPanel(self, 0);
    UiHologramGalleryTweenBoard(self, 0);
    UiHologramGalleryTweenCountPanel(self, 0);
    self->base.phaseStep++;
    break;
  case 1:
    done = UiHologramGalleryBaseDone(self, 0);
    done = (u8)(done + UiHologramGalleryMenuDone(self, 0));
    done = (u8)(done + UiHologramGalleryInfoPanelDone(self, 0));
    done = (u8)(done + UiHologramGalleryBoardDone(self, 0));
    done = (u8)(done + UiHologramGalleryCountPanelDone(self, 0));
    if (done == 5) {
      UiHologramGalleryStartSlotPulse(self, 1);
      self->base.phaseStep++;
    }
    break;
  case 2:
    if (self->firstVisitHelp >= 2) {
      UiHologramGalleryArmMessage(self, 8, 2, 2);
      self->firstVisitHelp = 0;
      self->base.phaseStep = 0x22;
    } else if (UiHologramGalleryCheckHint(self, 9) == 1) {
      self->base.phaseStep = 0x1f;
    } else if (UiHologramGalleryCheckHint(self, 1) == 1) {
      self->base.phaseStep = 0x1f;
    } else if (UiHologramGalleryCheckHint(self, 6) == 1) {
      self->base.phaseStep = 0x1f;
    } else {
      UiHologramGalleryUpdateCursor(self);
      self->base.phaseStep = 3;
    }
    break;
  case 3:
    UiHologramGalleryStartCursorPulse(self);
    UiHologramGalleryStopCursorPulse(self);
    UiHologramGalleryAnimateFocus(self);
    UiPulseStep(SPRITE(0xbc), (UiPulse *)&self->tweens[0xbc]);
    decide = (u8)UiHologramGalleryCheckDecide(self);
    if (decide != 0) {
      if (decide == 1) {
        PlayMenuSound(0);
        UiHologramGalleryUpdateCursor(self);
        UiHologramGalleryFlashSelection(self);
        self->exitMode = 0;
        self->base.phaseStep = 8;
      } else {
        PlayMenuSound(3);
      }
    } else if ((u8)UiHologramGalleryWantsHelp(self) != 0) {
      PlayMenuSound(2);
      UiHologramGalleryUpdateCursor(self);
      self->exitMode = 1;
      self->base.phaseStep = 4;
    } else if (UiHologramGalleryWantsAltAction(self) != 0) {
      PlayMenuSound(0);
      UiHologramGalleryUpdateCursor(self);
      self->exitMode = 1;
      UiHologramGalleryArmMessage(self, 0xb, 4, 3);
      self->base.phaseStep = 0x22;
    } else if (UiHologramGalleryMoveCursor(self) == 1) {
      PlayMenuSound(1);
      UiHologramGalleryUpdateCursor(self);
    }
    break;
  case 4:
    UiHologramGalleryHideCursor(self);
    UiHologramGalleryTweenBase(self, 1);
    UiHologramGalleryTweenMenu(self, 1);
    UiHologramGalleryTweenInfoPanel(self, 1);
    UiHologramGalleryTweenBoard(self, 1);
    UiHologramGalleryTweenCountPanel(self, 1);
    self->base.phaseStep++;
    break;
  case 5:
    done = UiHologramGalleryBaseDone(self, 1);
    done = (u8)(done + UiHologramGalleryMenuDone(self, 1));
    done = (u8)(done + UiHologramGalleryInfoPanelDone(self, 1));
    done = (u8)(done + UiHologramGalleryBoardDone(self, 1));
    done = (u8)(done + UiHologramGalleryCountPanelDone(self, 1));
    if (done == 5) {
      self->base.phaseStep++;
    }
    break;
  case 6:
    if (self->exitMode != 0 || self->menuCursor == 2) {
      SndBgmCancelChannel(0);
      SndBgmQueueStop(0.1f, 0);
    }
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
    GfxFaderStart(GfxGetActiveFader(), 0x10);
    self->base.phaseStep++;
    break;
  case 7:
    if (GfxFaderIsFinished(GfxGetActiveFader())) {
      self->base.phaseStep = 0x23;
    }
    break;
  case 8:
    if (UiHologramGalleryFlashDone(self) == 1) {
      UiHologramGalleryDecideMenu(self);
    }
    break;
  case 9:
    UiHologramGalleryHideCursor(self);
    UiHologramGalleryTweenWindow(self, 1);
    self->base.phaseStep++;
    break;
  case 10:
    if ((u8)UiHologramGallerySlotCursorDone(self, 1) == 1) {
      self->base.phaseStep++;
    }
    break;
  case 0xb:
    self->panel = 1;
    UiHologramGalleryRefreshMenu(self);
    UiHologramGalleryTweenSlotPage(self, 0);
    self->base.phaseStep++;
    break;
  case 0xc:
    if ((u8)UiHologramGallerySlotPageDone(self, 0) == 1) {
      UiHologramGalleryStartListPulse(self, 1);
      if (UiHologramGalleryCheckHint(self, 2) == 1) {
        self->base.phaseStep = 0x1f;
      } else {
        UiHologramGalleryUpdateCursor(self);
        self->base.phaseStep++;
      }
    }
    break;
  case 0xd:
    UiHologramGalleryStartCursorPulse(self);
    UiHologramGalleryStopCursorPulse(self);
    UiHologramGalleryAnimateFocus(self);
    UiPulseStep(SPRITE(0xbc), (UiPulse *)&self->tweens[0xbc]);
    decide = (u8)UiHologramGalleryCheckDecide(self);
    if (decide != 0) {
      if (decide == 1) {
        PlayMenuSound(0);
        UiHologramGalleryUpdateCursor(self);
        UiHologramGalleryFlashSelection(self);
        self->exitMode = 0;
        self->base.phaseStep = 8;
      } else {
        PlayMenuSound(3);
      }
    } else if ((u8)UiHologramGalleryWantsHelp(self) != 0) {
      PlayMenuSound(2);
      UiHologramGalleryUpdateCursor(self);
      self->exitMode = 1;
      UiHologramGalleryStartListPulse(self, 0);
      self->base.phaseStep = 0xe;
    } else if (UiHologramGalleryMoveCursor(self) == 1) {
      PlayMenuSound(1);
      UiHologramGalleryUpdateCursor(self);
      UiHologramGalleryStartListPulse(self, 1);
    }
    break;
  case 0xe:
    UiHologramGalleryHideCursor(self);
    UiHologramGalleryTweenSlotPage(self, 1);
    self->base.phaseStep++;
    break;
  case 0xf:
    if ((u8)UiHologramGallerySlotPageDone(self, 1) == 1) {
      self->base.phaseStep = (self->exitMode == 0) ? 0x14 : 0x12;
    }
    break;
  case 0x10:
    PlayMenuSound(2);
    UiHologramGalleryHideCursor(self);
    UiHologramGalleryResetConfirm(self);
    self->base.phaseStep++;
    break;
  case 0x11:
    if (UiHologramGalleryRemoveHologram(self) == 1) {
      UiHologramGalleryUpdateCursor(self);
      self->base.phaseStep = 0xd;
    }
    break;
  case 0x12:
    self->panel = 0;
    UiHologramGalleryRefreshMenu(self);
    UiHologramGalleryTweenWindow(self, 0);
    self->base.phaseStep++;
    break;
  case 0x13:
    if ((u8)UiHologramGallerySlotCursorDone(self, 0) == 1) {
      UiHologramGalleryUpdateCursor(self);
      self->base.phaseStep = 3;
    }
    break;
  case 0x14:
    UiHologramGalleryTweenHelpPanel(self, 1);
    self->base.phaseStep++;
    break;
  case 0x15:
    if ((u8)UiHologramGalleryHelpPanelDone(self, 1) == 1) {
      self->base.phaseStep = 0x18;
    }
    break;
  case 0x16:
    UiHologramGalleryTweenHelpPanel(self, 0);
    self->base.phaseStep++;
    break;
  case 0x17:
    if ((u8)UiHologramGalleryHelpPanelDone(self, 0) == 1) {
      self->base.phaseStep = 0xb;
    }
    break;
  case 0x18:
    self->panel = 2;
    UiHologramGalleryRefreshMenu(self);
    UiHologramGalleryTweenHologramList(self, 0);
    self->base.phaseStep++;
    break;
  case 0x19:
    if ((u8)UiHologramGalleryHologramListDone(self, 0) == 1) {
      UiHologramGalleryStartHelpPages(self, 1);
      if (UiHologramGalleryCheckHint(self, 3) == 1) {
        self->base.phaseStep = 0x1f;
      } else if (UiHologramGalleryCheckHint(self, 7) == 1) {
        self->base.phaseStep = 0x1f;
      } else {
        UiHologramGalleryUpdateCursor(self);
        self->base.phaseStep++;
      }
    }
    break;
  case 0x1a:
    UiHologramGalleryStartCursorPulse(self);
    UiHologramGalleryStopCursorPulse(self);
    UiHologramGalleryAnimateFocus(self);
    UiPulseStep(SPRITE(0xbc), (UiPulse *)&self->tweens[0xbc]);
    decide = (u8)UiHologramGalleryCheckDecide(self);
    if (decide != 0) {
      if (decide == 1) {
        PlayMenuSound(10);
        UiHologramGalleryUpdateCursor(self);
        UiHologramGalleryFlashSelection(self);
        self->exitMode = 0;
        self->base.phaseStep = 8;
      } else {
        UiHologramGalleryArmMessage(self, 6, 0x1a, 0x1a);
        PlayMenuSound(3);
        self->base.phaseStep = 0x22;
      }
    } else if ((u8)UiHologramGalleryWantsHelp(self) != 0) {
      PlayMenuSound(2);
      UiHologramGalleryUpdateCursor(self);
      self->exitMode = 1;
      UiHologramGalleryStartHelpPages(self, 0);
      self->base.phaseStep = 0x1b;
    } else if (UiHologramGalleryMoveHelpPage(self) == 1) {
      PlayMenuSound(1);
      UiHologramGalleryUpdateCursor(self);
    } else if (UiHologramGalleryMoveCursor(self) == 1) {
      PlayMenuSound(1);
      UiHologramGalleryUpdateCursor(self);
    }
    break;
  case 0x1b:
    UiHologramGalleryHideCursor(self);
    UiHologramGalleryTweenHologramList(self, 1);
    self->base.phaseStep++;
    break;
  case 0x1c:
    if ((u8)UiHologramGalleryHologramListDone(self, 1) == 1) {
      self->base.phaseStep = 0x16;
    }
    break;
  case 0x1d:
    UiHologramGalleryHideCursor(self);
    UiHologramGalleryStartHelpPages(self, 0);
    UiHologramGalleryResetConfirm(self);
    self->base.phaseStep++;
    break;
  case 0x1e:
    if (UiHologramGalleryPlaceHologram(self) == 1) {
      if (UiHologramGalleryCheckHint(self, 4) == 1) {
        self->base.phaseStep = 0x1f;
      } else {
        self->base.phaseStep = 0x1b;
      }
    }
    break;
  case 0x1f:
    self->base.phase = 3;
    self->base.phaseStep = 0;
    break;
  case 0x20:
    UiHologramGalleryStartBonus(self);
    self->base.phaseStep++;
    break;
  case 0x21:
    if (UiHologramGalleryCountBonus(self) == 1) {
      UiHologramGalleryUpdateCursor(self);
      self->base.phaseStep = 3;
    }
    break;
  case 0x22:
    if (UiHologramGalleryShowHelpDialog(self) == 1) {
      self->base.phaseStep = self->msgDeclined != 0 ? self->msgArgB : self->msgArgA;
    }
    break;
  default:
    UiHologramGallerySetExitResult(self);
    self->base.phase = 4;
    self->base.phaseStep = 0;
    break;
  }
  UiCellBlinkUpdate(&self->cellBlink);
  UiScrollLoopUpdate(&self->scrollLoop);
  UiHologramGalleryPulseSlots(self);
  UiHologramGalleryPulseList(self);
  UiHologramGalleryAnimateScrollArrows(self);
}
