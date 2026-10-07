// bdc 0x08942474 UiNetLobbyHostPhase
#include "bdc.h"

/* Host phase (entry 1 of the phase table `0x08a9cf78`) of the ad-hoc network lobby screen (task
   2000, `UiNetLobbyCtor`, created by the network menu task 1999): once the open animation is done
   and hosting has started (`NetPlayStartHost`), shows the list of joined players; confirm (pad bit
   0x4000) with at least 2 members starts the match (`NetPlaySetState(mgr, 3)`, sound 0), cancel (pad
   bit 0x2000) asks through the system dialog (`UiMsgWindowOpen(dlg, 1, 0x12)`) whether to stop; on
   yes it flags the local character and waits until it is alone, on a lost connection (profile flag
   0x80) it closes the panels and aborts the session (`NetPlayRequestAbort`). */

void UiNetLobbyHostPhase(UiNetLobby *self)
{
  bool go;
  bool yes;
  UiMsgWindow *dlg;
  NetChara *chara;
  s32 choice;
  bool doneA;
  bool doneB;

  switch (self->base.phaseStep) {
  case 1:
    /* wait for the open animation, then start hosting */
    go = true;
    if (UiNetLobbyStubDoneA(&self->base, 0) == 0) {
      go = false;
    }
    if (UiNetLobbyShowPanels(self, 0) == 0) {
      go = false;
    }
    if (!go) {
      return;
    }
    if (!NetPlayStartHost((NetPlay *)NetPlayGetManager())) {
      return;
    }
    self->base.phaseStep = 2;
    UiNetLobbyDrawPlayerList(self, true, false, false, true);
    return;

  case 2:
    /* lobby input */
    if (NetStatusIsShown() == 0) {
      go = false;
      if (!NetPlayHasManager()) {
        SaveProfileSetFlags(SaveGetProfile(), 0x80);
        go = true;
      } else if ((self->base.pad->pressed & 0x2000) != 0) {
        if (SndHasManager()) {
          SndManagerPlay(SndGetManager(), 2, 0, 0);
        }
        if (!UiMsgWindowExists()) {
          UiMsgWindowEnsure();
        }
        ((UiMsgWindow *)UiMsgWindowGet())->mode = 0;
        UiMsgWindowOpen((UiMsgWindow *)UiMsgWindowGet(), 1, 0x12);
        self->base.phaseStep = 3;
      } else if (SaveHasProfile()) {
        if (SaveProfileHasFlags(SaveGetProfile(), 0x80)) {
          go = true;
        }
      }
      if (go) {
        /* connection lost: close and abort */
        self->base.phaseStep = 5;
        UiNetLobbyStubDoneB(&self->base, 1);
        UiNetLobbyHidePanels(self, 1);
        if (NetPlayHasManager()) {
          NetPlayRequestAbort((NetPlay *)NetPlayGetManager());
        }
      } else if ((self->base.pad->pressed & 0x4000) != 0) {
        go = false;
        if (NetPlayHasManager()) {
          if (NetPlayGetPeerCount((NetPlay *)NetPlayGetManager()) >= 2) {
            go = true;
          }
        }
        if (go) {
          if (SndHasManager()) {
            SndManagerPlay(SndGetManager(), 0, 0, 0);
          }
          self->base.phaseStep = 4;
          UiNetLobbyStubDoneB(&self->base, 1);
          UiNetLobbyHidePanels(self, 1);
          NetPlaySetState((NetPlay *)NetPlayGetManager(), 3);
        } else if (SndHasManager()) {
          SndManagerPlay(SndGetManager(), 3, 0, 0);
        }
      }
    }
    UiNetLobbyDrawPlayerList(self, true, false, false, false);
    return;

  case 3:
    /* wait for the "stop hosting?" dialog answer */
    go = true;
    yes = true;
    if (UiMsgWindowExists()) {
      yes = false;
      if (UiMsgWindowIsClosed((UiMsgWindow *)UiMsgWindowGet())) {
        choice = UiMsgWindowGetChoice((UiMsgWindow *)UiMsgWindowGet());
        yes = (choice == 0);
      } else {
        go = false;
        if (SaveHasProfile() && SaveProfileHasFlags(SaveGetProfile(), 0x80)) {
          UiMsgWindowRequestClose((UiMsgWindow *)UiMsgWindowGet());
        }
      }
    }
    if (!go) {
      return;
    }
    self->base.phaseStep = 2;
    if (!yes) {
      return;
    }
    self->base.phaseStep = 6;
    chara = NetCharaGetByIndex(0);
    if (chara != NULL) {
      NetCharaSetLobbyFlag(chara);
    }
    return;

  case 4:
    /* match starting: wait for the close animation and the session state */
    go = true;
    if (UiNetLobbyStubDoneB(&self->base, 0) == 0) {
      go = false;
    }
    if (UiNetLobbyHidePanels(self, 0) == 0) {
      go = false;
    }
    if (!go) {
      return;
    }
    go = false;
    if (!NetPlayHasManager()) {
      self->base.phaseStep = 5;
      SaveProfileSetFlags(SaveGetProfile(), 0x80);
    } else if (NetPlayGetState((NetPlay *)NetPlayGetManager()) == 7) {
      go = true;
    }
    if (!go) {
      return;
    }
    self->base.phase = 3;
    self->base.phaseStep = 0;
    return;

  case 5:
    /* closing: leave once the panels are hidden and the manager is gone */
    doneA = UiNetLobbyStubDoneB(&self->base, 0) != 0;
    doneB = UiNetLobbyHidePanels(self, 0) != 0;
    go = doneA && doneB;
    if (NetPlayHasManager()) {
      go = false;
    }
    if (!go) {
      return;
    }
    self->base.phase = 3;
    self->leaving = 1;
    self->base.phaseStep = -1;
    return;

  case 6:
    /* wait until the local character is alone, then close and abort */
    go = true;
    chara = NetCharaGetByIndex(0);
    if (chara != NULL) {
      go = false;
      if (NetCharaIsLobbyFlagAlone(chara) != 0) {
        go = true;
      }
    }
    if (!go) {
      return;
    }
    self->base.phaseStep = 5;
    UiNetLobbyStubDoneB(&self->base, 1);
    UiNetLobbyHidePanels(self, 1);
    if (NetPlayHasManager()) {
      NetPlayRequestAbort((NetPlay *)NetPlayGetManager());
    }
    return;

  default:
    /* setup */
    UiNetLobbySetField7c(self, 0x13);
    UiNetLobbyStubDoneA(&self->base, 1);
    UiNetLobbySetActivePanel(self, 1);
    UiNetLobbyShowPanels(self, 1);
    NetPlayCreate();
    self->base.phaseStep = 1;
    return;
  }
}
