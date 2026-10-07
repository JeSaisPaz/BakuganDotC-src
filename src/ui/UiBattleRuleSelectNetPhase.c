// bdc 0x089557d0 UiBattleRuleSelectNetPhase
#include "bdc.h"

/* Net-play variant of `UiBattleRuleSelectMainPhase` (entry 4 of the phase table `0x08a9d4e0`)
   of the battle-rule menu (task 340, `UiBattleRuleSelectCtor`; four battle types, reached from
   the battle-mode screen 350; the class purpose is inferred from what it writes to the save
   profile). While `SaveGetProfileFlag0` is set, sub-states 3, 10 and 16 (and anything >= 20)
   only run on frames where NetPlay flag 0x2000000 is set and `NetCharaIsSyncHandshakeDone`, or
   where the peer's record could be read; every such waiting frame (NetPlay present, character 0
   present) it reads the peer slot (`!netPlayer`) of character 0 into `netState[peer]` when the
   record carries flag 0x2000000, notes whether both slots report sub-state 0x10, and pushes its
   own `{cursor, subCursor, phaseStep, cancelled}` with `NetCharaPushMessage`. Profile flag 0x80
   in sub-states 3/10/16 opens message 0x1d and goes to sub-state 0x11. Only player 0 takes input
   in sub-state 3; player 1 goes straight to 0x10, where both follow player 0's cursor
   (`netState[0]`) and, once both report 0x10, confirm or cancel (cancel requests a NetPlay abort,
   player 1 also sees message 0x1e). Out-of-range sub-states: `UiBattleRuleSelectCommit` and
   phase 3. */

/* 0x28-byte NetChara message record as used by this menu. */
typedef struct RuleSelectNetMsg {
  u32 word0;
  u32 flags;      /* 0x2000000: record carries a menu state */
  u8 state[4];    /* cursor, subCursor, phaseStep, cancelled */
  u32 rest[7];
} RuleSelectNetMsg;

static void PlaySe(u32 soundId)
{
  if (SndHasManager()) {
    SndManagerPlay(SndGetManager(), soundId, 0, 0);
  }
}

static void OpenMsgWindow(s32 mode, s32 openMode, s32 msgIndex)
{
  if (!UiMsgWindowExists()) {
    UiMsgWindowEnsure();
  }
  ((UiMsgWindow *)UiMsgWindowGet())->mode = mode;
  UiMsgWindowOpen((UiMsgWindow *)UiMsgWindowGet(), openMode, msgIndex);
}

void UiBattleRuleSelectNetPhase(UiBattleRuleSelect *self)
{
  bool run = true;
  bool bothAtConfirm = false;
  bool cancel;
  NetChara *chara;
  s32 peer;
  s32 me;
  s32 step;
  s32 choice;
  u8 done;
  u8 result;
  RuleSelectNetMsg msg;

  if (SaveGetProfileFlag0()) {
    switch (self->base.phaseStep) {
    case 3:
    case 10:
    case 16:
      run = false;
      break;
    default:
      run = (u32)self->base.phaseStep < 20;
      break;
    }
    if (NetPlayHasManager() && !run) {
      chara = NetCharaGetByIndex(0);
      if ((NetPlayGetFlags((NetPlay *)NetPlayGetManager()) & 0x2000000) != 0 &&
          NetCharaIsSyncHandshakeDone()) {
        run = true;
      }
      if (!run && chara != NULL) {
        if (NetPlayIsSynced((NetPlay *)NetPlayGetManager())) {
          peer = (self->netPlayer == 0) ? 1 : 0;
          if (NetCharaReadSlot(chara, peer, (u32 *)&msg)) {
            run = true;
            if ((msg.flags & 0x2000000) != 0) {
              self->netState[peer][0] = msg.state[0];
              self->netState[peer][1] = msg.state[1];
              self->netState[peer][2] = msg.state[2];
              self->netState[peer][3] = msg.state[3];
            }
          }
          if (NetCharaReadSlot(chara, self->netPlayer, (u32 *)&msg) && self->netState[peer][2] == 0x10 &&
              msg.state[2] == 0x10) {
            bothAtConfirm = true;
          }
        }
        memset(&msg, 0, sizeof(msg));
        me = self->netPlayer;
        msg.flags = 0x2000000;
        self->netState[me][0] = (u8)self->cursor;
        self->netState[me][1] = (u8)self->subCursor;
        self->netState[me][2] = (u8)self->base.phaseStep;
        self->netState[me][3] = self->cancelled;
        memcpy(msg.state, self->netState[me], 4);
        NetCharaPushMessage(chara, (u32 *)&msg);
      }
    }
    if (SaveHasProfile() && SaveProfileHasFlags(SaveGetProfile(), 0x80)) {
      step = self->base.phaseStep;
      if (step == 0x10 || step == 10 || step == 3) {
        OpenMsgWindow(1, 0, 0x1d);
        self->base.phaseStep = 0x11;
      }
    }
  }
  if (!run) {
    return;
  }

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
    if (self->netPlayer != 0) {
      self->base.phaseStep = 0x10;
      break;
    }
    result = (u8)UiBattleRuleSelectCheckConfirm(self);
    if (result == 0) {
      if ((self->base.pad->pressed & 0x2000) != 0) {
        if (!SaveGetProfileFlag0()) {
          self->cancelled = 1;
          self->base.phaseStep = 0x10;
          break;
        }
        PlaySe(2);
        OpenMsgWindow(0, 1, 0x11);
        self->base.phaseStep = 0x12;
        break;
      }
      if (UiBattleRuleSelectMoveCursor(self) == 1) {
        PlaySe(1);
        UiBattleRuleSelectStartCursorMove(self);
        self->base.phaseStep = 4;
      }
    } else if (result == 1) {
      self->cancelled = 0;
      self->base.phaseStep = 0x10;
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
        self->base.phaseStep = 0x10;
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
  case 0x10:
    if (self->cursor != (s8)self->netState[0][0]) {
      /* follow player 0's cursor */
      self->prevCursor = self->cursor;
      self->cursor = (s8)self->netState[0][0];
      memset(&self->switchDir, 0, 0xc);
      self->switchFrames = 8.0f;
      self->switchDir = ((self->prevCursor + 1) % 4) != self->cursor;
      PlaySe(1);
      UiBattleRuleSelectStartCursorMove(self);
      self->base.phaseStep = 4;
    } else if (bothAtConfirm) {
      if (self->netState[0][3] == 0) {
        PlaySe(0);
        UiBattleRuleSelectStartConfirmFlash(self);
        self->cancelled = 0;
        self->base.phaseStep = 7;
      } else {
        PlaySe(2);
        self->cancelled = 1;
        self->base.phaseStep = 5;
        if (self->netPlayer == 1) {
          OpenMsgWindow(1, 0, 0x1e);
          self->base.phaseStep = 0x11;
        }
        if (NetPlayHasManager()) {
          NetPlayRequestAbort((NetPlay *)NetPlayGetManager());
        }
      }
    }
    break;
  case 0x11:
    done = 1;
    if (UiMsgWindowExists()) {
      done = 0;
      if (UiMsgWindowIsClosed((UiMsgWindow *)UiMsgWindowGet())) {
        done = 1;
      }
    }
    if (done) {
      self->base.phaseStep = 5;
    }
    break;
  case 0x12:
    done = 1;
    cancel = true;
    if (UiMsgWindowExists()) {
      cancel = false;
      if (UiMsgWindowIsClosed((UiMsgWindow *)UiMsgWindowGet())) {
        /* choice 0 confirms the cancel; -1, 1 and anything else return to the menu */
        choice = UiMsgWindowGetChoice((UiMsgWindow *)UiMsgWindowGet());
        cancel = (choice == 0);
      } else {
        done = 0;
        if (SaveHasProfile() && SaveProfileHasFlags(SaveGetProfile(), 0x80)) {
          UiMsgWindowRequestClose((UiMsgWindow *)UiMsgWindowGet());
        }
      }
    }
    if (done) {
      if (cancel) {
        self->cancelled = 1;
        self->base.phaseStep = 0x10;
      } else {
        self->base.phaseStep = 3;
      }
    }
    break;
  default:
    if (SaveProfileHasFlags(SaveGetProfile(), 0x80)) {
      self->cancelled = 1;
    }
    UiBattleRuleSelectCommit(self);
    self->base.phase = 3;
    self->base.phaseStep = 0;
    break;
  }
}
