// bdc 0x0884f058 BtlMainUpdateQuitPrompt
#include "bdc.h"

/* Quit/abort prompt of the battle main task (task id 100, `BtlMainTaskCtor`, vtable `0x08af18f4`;
   the task `BtlGetCameraTask` returns).
   Netplay (`SaveGetProfileFlag0` set): driven by profile flags 0x200 (prompt open), 0x2000 (quit
   chosen), 0x800 (quit done) and 0x80 (cancel all). START (pad bit 0x8) opens the quit message
   (message 26, `UiMsgWindowOpen`) only while START is allowed: phase 1, UI window 0xc active and
   profile word 2 equal to -1 or >= 301; otherwise the prompt is blocked, and with word 2 != -1 and
   < 301 an open prompt is closed unless quit (0x2000) was already chosen. Choosing "yes" (choice 0) starts the `quitStep` counter;
   when it exceeds 300, or the peer flags 0x200000 on both net records (`NetCharaBothHaveFlags`),
   the battle is aborted once: `NetPlayRequestAbort`, `g_btlBattleOutcome` = 7, profile flag
   0x400, phase/drawPhase 2 with phaseStep 100, `BtlStopBgm` and all voices faded out. Always
   returns 0 in netplay.
   Offline: returns 1 when script global bit 31 is set, START was pressed and UI window 0xb is
   active, else 0. */

int BtlMainUpdateQuitPrompt(BtlMain *self)
{
  int result = 0;
  bool startQuit = false;

  if (SaveGetProfileFlag0() == 0) {
    if (CoreBitsetTest(0x1f, g_scriptGlobalBits) == 1 && ((s8)self->pad->pressed & 8) != 0) {
      startQuit = true;
    }
  } else {
    bool promptBlocked = false; /* false: START may open the prompt */
    bool wordValid = false;
    bool forceQuit;
    NetChara *chara;

    if (self->phase == 1) {
      s32 word = (s32)SaveProfileGetWord(SaveGetProfile(), 2);
      if (UiGetWindowActive(0xc) == 0) {
        promptBlocked = true;
      } else if (word != -1 && word < 0x12d) {
        promptBlocked = true;
        wordValid = true;
      }
    } else {
      promptBlocked = true;
    }

    if (promptBlocked) {
      if (wordValid) {
        if (SaveProfileHasFlags(SaveGetProfile(), 0x200) &&
            !SaveProfileHasFlags(SaveGetProfile(), 0x2000)) {
          SaveProfileClearFlags(SaveGetProfile(), 0x200);
          if (UiMsgWindowExists() && !UiMsgWindowIsClosed(UiMsgWindowGet())) {
            UiMsgWindowRequestClose(UiMsgWindowGet());
          }
          self->quitStep = 0;
        }
      } else if (SaveProfileHasFlags(SaveGetProfile(), 0x200)) {
        SaveProfileClearFlags(SaveGetProfile(), 0x200);
        if (UiMsgWindowExists() && !UiMsgWindowIsClosed(UiMsgWindowGet())) {
          UiMsgWindowRequestClose(UiMsgWindowGet());
        }
        self->quitStep = 0;
        return 0;
      }
    }

    if (SaveProfileHasFlags(SaveGetProfile(), 0x80)) {
      SaveProfileClearFlags(SaveGetProfile(), 0x200);
      SaveProfileClearFlags(SaveGetProfile(), 0x2000);
      return 0;
    }

    forceQuit = false;
    if (self->quitStep > 0) {
      if (self->quitStep < 0x12d) {
        self->quitStep = self->quitStep + 1;
      } else {
        forceQuit = true;
      }
    }

    chara = NetCharaGetByIndex(0);
    if (chara != NULL) {
      if (NetCharaBothHaveFlags(chara, 0x200000)) {
        forceQuit = true;
      } else if (NetCharaSlotsHaveFlag(chara, 0x400000)) {
        SaveProfileSetFlags(SaveGetProfile(), 0x1000);
      }
    }

    if (forceQuit && !SaveProfileHasFlags(SaveGetProfile(), 0x800)) {
      SaveProfileSetFlags(SaveGetProfile(), 0x800);
      if (NetPlayHasManager()) {
        NetPlayRequestAbort(NetPlayGetManager());
      }
      g_btlBattleOutcome = 7;
      SaveProfileSetFlags(SaveGetProfile(), 0x400);
      self->phase = 2;
      self->drawPhase = 2;
      self->phaseStep = 100;
      BtlStopBgm();
      SndManagerFadeOutAllVoices(SndGetManager());
      self->quitStep = 0;
    }

    if (SaveProfileHasFlags(SaveGetProfile(), 0x3800)) {
      return 0;
    }

    if (SaveProfileHasFlags(SaveGetProfile(), 0x200)) {
      bool answered = true;
      if (UiMsgWindowExists() && !UiMsgWindowIsClosed(UiMsgWindowGet()) &&
          !UiMsgWindowHasChoice(UiMsgWindowGet())) {
        answered = false;
      }
      if (answered) {
        self->quitStep = 0;
        SaveProfileClearFlags(SaveGetProfile(), 0x200);
        if (UiMsgWindowExists() && UiMsgWindowGetChoice(UiMsgWindowGet()) == 0) {
          SaveProfileSetFlags(SaveGetProfile(), 0x2000);
          self->quitStep = 1;
        }
      }
    } else if (!promptBlocked && ((s8)self->pad->pressed & 8) != 0) {
      UiMsgWindow *win;

      SaveProfileSetFlags(SaveGetProfile(), 0x200);
      if (!UiMsgWindowExists()) {
        UiMsgWindowEnsure();
      }
      win = UiMsgWindowGet();
      win->mode = 0;
      win = UiMsgWindowGet();
      win->depth = 26000.0f;
      UiMsgWindowOpen(UiMsgWindowGet(), 1, 0x1a);
    }
    return 0;
  }

  if (startQuit && UiGetWindowActive(0xb) != 0) {
    result = 1;
  }
  return result;
}
