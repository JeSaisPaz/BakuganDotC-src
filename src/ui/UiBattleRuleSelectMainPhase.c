// bdc 0x0895513c UiBattleRuleSelectMainPhase
#include "bdc.h"

/* Offline main phase (entry 2 of the phase table `0x08a9d4e0`) of the battle-rule menu (task 340,
   `UiBattleRuleSelectCtor`; four battle types, reached from the battle-mode screen 350; the class
   purpose is inferred from what it writes to the save profile): opens buttons/panels, handles
   cursor movement and confirm; entry 3 opens a sub-selector (three extra parts,
   `UiBattleRuleSelectStartSubWindow`/`UiBattleRuleSelectStartSubOptionTweens`/`UiBattleRuleSelectStartSubLabelTweens`, value input `UiBattleRuleSelectMoveSubCursor`) before confirming;
   cancel (pad `pressed` 0x2000) closes with `cancelled` = 1. When `phaseStep` is out of range
   calls `UiBattleRuleSelectCommit`, resets `phaseStep` and advances `phase`. */

static void PlaySe(u32 soundId)
{
  if (SndHasManager()) {
    SndManagerPlay(SndGetManager(), soundId, 0, 0);
  }
}

void UiBattleRuleSelectMainPhase(UiBattleRuleSelect *self)
{
  u8 done;
  u8 result;

  switch (self->base.phaseStep) {
  case 0:
    UiBattleRuleSelectStartButtonTween(self, false);
    self->base.phaseStep++;
    break;
  case 1:
    if ((u8)UiBattleRuleSelectUpdateButtonTween(self, false) == 1) {
      UiBattleRuleSelectStartArrowSlide(self, 0);
      self->base.phaseStep++;
    }
    break;
  case 2:
    if ((u8)UiBattleRuleSelectUpdatePanelTween(self, false) == 1) {
      UiBattleRuleSelectResetButtonGlow(self, 1, (u8)self->cursor);
      self->base.phaseStep++;
    }
    break;
  case 3:
    UiBattleRuleSelectPulseButtonGlow(self, (u8)self->cursor);
    result = (u8)UiBattleRuleSelectCheckConfirm(self);
    if (result == 0) {
      if ((self->base.pad->pressed & 0x2000) != 0) {
        PlaySe(2);
        self->cancelled = 1;
        self->base.phaseStep = 5;
        return;
      }
      if (UiBattleRuleSelectMoveCursor(self) == 1) {
        PlaySe(1);
        UiBattleRuleSelectStartCursorMove(self);
        self->base.phaseStep = 4;
      }
    } else if (result == 1) {
      PlaySe(0);
      UiBattleRuleSelectStartConfirmFlash(self);
      self->cancelled = 0;
      self->base.phaseStep = 7;
      return;
    } else {
      PlaySe(3);
    }
    break;
  case 4:
    if (UiBattleRuleSelectAnimateCursorMove(self) == 1) {
      UiBattleRuleSelectResetButtonGlow(self, 1, (u8)self->cursor);
      self->base.phaseStep = 3;
    }
    break;
  case 5:
    UiBattleRuleSelectStartButtonTween(self, true);
    UiBattleRuleSelectStartArrowSlide(self, 1);
    self->base.phaseStep++;
    break;
  case 6:
    done = (u8)UiBattleRuleSelectUpdateButtonTween(self, true);
    done = (u8)(done + UiBattleRuleSelectUpdatePanelTween(self, true));
    if (done == 2) {
      self->base.phaseStep = 0x13;
    }
    break;
  case 7:
    if (UiBattleRuleSelectConfirmFlashDone() == 1) {
      self->base.phaseStep = (self->cursor == 3) ? 8 : 5;
    }
    break;
  case 8:
    UiBattleRuleSelectResetButtonGlow(self, 0, (u8)self->cursor);
    UiBattleRuleSelectStartSubWindow(self, 0);
    UiBattleRuleSelectStartSubOptionTweens(self, 0);
    UiBattleRuleSelectStartSubLabelTweens(self, 0);
    self->base.phaseStep++;
    break;
  case 9:
    done = (u8)UiBattleRuleSelectUpdateSubWindow(self, 0);
    done = (u8)(done + UiBattleRuleSelectSubOptionTweensDone(self, 0));
    done = (u8)(done + UiBattleRuleSelectSubLabelTweensDone(self, 0));
    if (done == 3) {
      self->subCursor = 0;
      UiBattleRuleSelectResetSubCursor(self);
      self->base.phaseStep++;
    }
    break;
  case 10:
    UiBattleRuleSelectPulseSubCursor(self);
    UiBattleRuleSelectAnimateSubFocus(self);
    UiPulseStep(((GfxSprite **)self->base.data)[0x1c], (UiPulse *)&self->tweens[28]);
    result = (u8)UiBattleRuleSelectCheckSubConfirm(self);
    if (result == 0) {
      if ((self->base.pad->pressed & 0x2000) != 0) {
        PlaySe(2);
        UiBattleRuleSelectResetSubCursor(self);
        UiBattleRuleSelectHideSubCursor(self);
        self->base.phaseStep = 0xb;
        return;
      }
      if (UiBattleRuleSelectMoveSubCursor(self) == 1) {
        PlaySe(1);
        UiBattleRuleSelectResetSubCursor(self);
      }
    } else if (result == 1) {
      PlaySe(0);
      UiBattleRuleSelectResetSubCursor(self);
      self->cancelled = 0;
      UiBattleRuleSelectStartSubConfirmFlash(self);
      self->base.phaseStep = 0xd;
    } else {
      PlaySe(3);
    }
    break;
  case 0xb:
    UiBattleRuleSelectStartSubWindow(self, 1);
    UiBattleRuleSelectStartSubOptionTweens(self, 1);
    UiBattleRuleSelectStartSubLabelTweens(self, 1);
    self->base.phaseStep++;
    break;
  case 0xc:
    done = (u8)UiBattleRuleSelectUpdateSubWindow(self, 1);
    done = (u8)(done + UiBattleRuleSelectSubOptionTweensDone(self, 1));
    done = (u8)(done + UiBattleRuleSelectSubLabelTweensDone(self, 1));
    if (done == 3) {
      UiBattleRuleSelectResetButtonGlow(self, 1, (u8)self->cursor);
      self->base.phaseStep = 3;
    }
    break;
  case 0xd:
    if (UiBattleRuleSelectSubConfirmFlashDone() == 1) {
      UiBattleRuleSelectHideSubCursor(self);
      UiBattleRuleSelectStartSubWindow(self, 1);
      UiBattleRuleSelectStartSubOptionTweens(self, 1);
      UiBattleRuleSelectStartSubLabelTweens(self, 1);
      self->base.phaseStep++;
    }
    break;
  case 0xe:
    done = (u8)UiBattleRuleSelectUpdateSubWindow(self, 1);
    done = (u8)(done + UiBattleRuleSelectSubOptionTweensDone(self, 1));
    done = (u8)(done + UiBattleRuleSelectSubLabelTweensDone(self, 1));
    if (done == 3) {
      self->base.phaseStep = 5;
    }
    break;
  case 0xf:
    if (UiCommonNoticeRun() == 1) {
      self->base.phaseStep = 3;
    }
    break;
  default:
    UiBattleRuleSelectCommit(self);
    self->base.phaseStep = 0;
    self->base.phase++;
    break;
  }
}
