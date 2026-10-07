// bdc 0x08942a18 UiNetLobbyJoinPhase
#include "bdc.h"

/* Join phase (entry 2 of the phase table `0x08a9cf78`) of the ad-hoc network lobby screen (task
   2000, `UiNetLobbyCtor`, created by the network menu task 1999), driven by `phaseStep`:
   starts joining (`NetPlayStartJoin`), lets the player pick a host from the list (cursor
   `joinCursor`), connects to it (`NetPlaySelectHost(mgr, joinCursor)`), waits for the host to start
   (NetPlay state 6 -> phase 3) or refuse/vanish, and shows system dialogs (0x12 quit?, 0x11 cancel?,
   0x15 full, 0x1c lost) for errors and cancellation; on the way back it notifies the network menu
   (task 1999, `UiNetMenuSetHelpText(task, 3|5)`). Leaving the lobby sets `leaving`, phase 3 and
   step -1. */

void UiNetLobbyJoinPhase(UiNetLobby *self)
{
  NetCharaPacketHeader hdr;
  NetPlayPeer *peer;
  bool ok;
  bool redraw;
  bool leave;
  bool full;
  bool refused;
  bool select;
  bool lost;
  bool backToList;
  bool notFound;
  bool confirmed;
  s32 move;
  s32 count;
  s32 cursor;
  s32 peerCount;
  s32 i;
  void *task;

  ok = true;
  redraw = false;
  switch (self->base.phaseStep) {
  default: /* 0, 8, 11: start */
    UiNetLobbySetField7c(self, 0x15);
    UiNetLobbyStubDoneA(&self->base, 1);
    UiNetLobbySetActivePanel(self, 0);
    UiNetLobbyShowPanels(self, 1);
    NetPlayCreate();
    self->base.phaseStep = 1;
    break;

  case 1: /* panels slide in, then start joining */
    if (UiNetLobbyStubDoneA(&self->base, 0) == 0) {
      ok = false;
    }
    if (UiNetLobbyShowPanels(self, 0) == 0) {
      ok = false;
    }
    if (ok && NetPlayStartJoin((NetPlay *)NetPlayGetManager())) {
      self->joinCursor = 0;
      GfxRectSetVisible(self->highlightRect, 1);
      self->base.phaseStep = 2;
      UiNetLobbyDrawPlayerList(self, true, true, true, true);
    }
    break;

  case 2: /* host list input */
    if (NetStatusIsShown() == 0) {
      leave = false;
      if (!NetPlayHasManager()) {
        leave = true;
      } else if ((self->base.pad->pressed & 0x2000) != 0) {
        /* cancel: ask "quit?" */
        if (SndHasManager()) {
          SndManagerPlay(SndGetManager(), 2, 0, 0);
        }
        if (!UiMsgWindowExists()) {
          UiMsgWindowEnsure();
        }
        ((UiMsgWindow *)UiMsgWindowGet())->mode = 0;
        UiMsgWindowOpen((UiMsgWindow *)UiMsgWindowGet(), 1, 0x12);
        self->base.phaseStep = 0xc;
      } else if (SaveHasProfile() && SaveProfileHasFlags(SaveGetProfile(), 0x80)) {
        leave = true;
      }

      if (leave) {
        self->base.phaseStep = 0x10;
        GfxRectSetVisible(self->highlightRect, 0);
        UiNetLobbyStubDoneB(&self->base, 1);
        UiNetLobbyHidePanels(self, 1);
        if (NetPlayHasManager()) {
          NetPlayRequestAbort((NetPlay *)NetPlayGetManager());
        }
      } else {
        move = 0;
        count = NetPlayGetPeerCount((NetPlay *)NetPlayGetManager());
        if ((self->base.pad->pressed & 0x4000) != 0) {
          /* confirm: check the host's lobby record before joining */
          select = false;
          full = false;
          refused = false;
          peer = (NetPlayPeer *)NetPlayGetPeer((NetPlay *)NetPlayGetManager(), self->joinCursor);
          peerCount = -1;
          if (peer != NULL) {
            i = 1;
            do {
              if (!NetCharaGetRecvHeader(&hdr, i)) {
                break;
              }
              if (hdr.mac[0] == peer->mac[0] && hdr.mac[1] == peer->mac[1] &&
                  hdr.mac[2] == peer->mac[2] && hdr.mac[3] == peer->mac[3] &&
                  hdr.mac[4] == peer->mac[4] && hdr.mac[5] == peer->mac[5]) {
                peerCount = hdr.peerCount;
              }
              i++;
            } while (i < 16);
            if (peerCount >= 2) {
              full = true;
            } else if (peerCount >= 0 && (hdr.flags & 1) != 0) {
              refused = true;
            }
          }

          if (refused) {
            if (SndHasManager()) {
              SndManagerPlay(SndGetManager(), 3, 0, 0);
            }
            self->base.phaseStep = 6;
            if (NetPlayHasManager()) {
              NetPlaySetState((NetPlay *)NetPlayGetManager(), 4);
              NetPlayClearSelectedHost((NetPlay *)NetPlayGetManager());
            }
          } else if (full) {
            if (SndHasManager()) {
              SndManagerPlay(SndGetManager(), 3, 0, 0);
            }
            if (!UiMsgWindowExists()) {
              UiMsgWindowEnsure();
            }
            ((UiMsgWindow *)UiMsgWindowGet())->mode = 1;
            UiMsgWindowOpen((UiMsgWindow *)UiMsgWindowGet(), 0, 0x15);
            self->base.phaseStep = 0xe;
          } else {
            if (count > 0 && self->joinCursor < count) {
              select = true;
            }
            if (select) {
              if (SndHasManager()) {
                SndManagerPlay(SndGetManager(), 0, 0, 0);
              }
              redraw = true;
              NetPlaySelectHost((NetPlay *)NetPlayGetManager(), self->joinCursor);
              UiNetLobbyStubDoneB(&self->base, 1);
              self->base.phaseStep = 3;
            } else if (SndHasManager()) {
              SndManagerPlay(SndGetManager(), 3, 0, 0);
            }
          }
        } else if ((self->base.pad->repeat & 0x10) != 0) {
          move = -1;
        } else if ((self->base.pad->repeat & 0x40) != 0) {
          move = 1;
        }

        if (move != 0) {
          redraw = true;
          cursor = move + self->joinCursor;
          if (cursor < 0) {
            cursor = 0;
          }
          if (count - 1 < cursor) {
            cursor = count - 1;
          }
          if (self->joinCursor != cursor) {
            self->joinCursor = cursor;
            if (SndHasManager()) {
              SndManagerPlay(SndGetManager(), 1, 0, 0);
            }
          } else if (SndHasManager()) {
            SndManagerPlay(SndGetManager(), 3, 0, 0);
          }
        }
        cursor = self->joinCursor;
        if (cursor >= count) {
          cursor = count - 1;
          if (cursor < 0) {
            cursor = 0;
          }
          self->joinCursor = cursor;
          redraw = true;
        }
        self->joinCursor = (cursor >= 0) ? cursor : 0;
      }
    }
    UiNetLobbyDrawPlayerList(self, true, true, true, redraw);
    break;

  case 3: /* host picked: wait for the panels, then wait for the host */
    UiNetLobbyDrawPlayerList(self, false, false, false, false);
    if (UiNetLobbyStubDoneB(&self->base, 0) == 0) {
      ok = false;
    }
    if (ok) {
      self->base.phaseStep = 5;
      task = CoreTaskFind(1999);
      if (task != NULL) {
        UiNetMenuSetHelpText(task, 5);
      }
      self->msgTimer = 150;
    }
    break;

  case 4:
    if (UiNetLobbyStubDoneA(&self->base, 0) != 0) {
      self->base.phaseStep = 5;
    }
    break;

  case 5: /* connected to the host: wait for it to start */
    backToList = false;
    lost = false;
    UiMenuFlagsModify(1, 1);
    if (!NetPlayHasManager()) {
      SaveProfileSetFlags(SaveGetProfile(), 0x80);
      backToList = true;
    } else if ((self->base.pad->pressed & 0x2000) != 0) {
      /* cancel: ask "cancel?" */
      if (!UiMsgWindowExists()) {
        UiMsgWindowEnsure();
      }
      ((UiMsgWindow *)UiMsgWindowGet())->mode = 0;
      UiMsgWindowOpen((UiMsgWindow *)UiMsgWindowGet(), 1, 0x11);
      self->base.phaseStep = 0xd;
      self->msgTimer = 150;
    } else {
      switch (NetPlayGetState((NetPlay *)NetPlayGetManager())) {
      case 5: /* join wait: the host must still announce us */
        notFound = true;
        peer = (NetPlayPeer *)NetPlayGetSelectedHost((NetPlay *)NetPlayGetManager());
        if (peer != NULL) {
          i = 1;
          do {
            if (NetCharaGetRecvHeader(&hdr, i) &&
                hdr.mac[0] == peer->mac[0] && hdr.mac[1] == peer->mac[1] &&
                hdr.mac[2] == peer->mac[2] && hdr.mac[3] == peer->mac[3] &&
                hdr.mac[4] == peer->mac[4] && hdr.mac[5] == peer->mac[5]) {
              notFound = false;
              break;
            }
            i++;
          } while (i < 16);
          if (!notFound && (hdr.flags & 1) != 0) {
            lost = true;
          }
        }
        if (notFound) {
          if (self->msgTimer <= 0) {
            lost = true;
          } else {
            self->msgTimer = self->msgTimer - 1;
          }
        } else {
          self->msgTimer = 150;
        }
        break;
      case 6: /* host launched */
        self->base.phaseStep = 0xf;
        GfxRectSetVisible(self->highlightRect, 0);
        UiNetLobbyStubDoneB(&self->base, 1);
        UiNetLobbyHidePanels(self, 1);
        break;
      default:
        if (self->msgTimer <= 0) {
          lost = true;
        } else {
          self->msgTimer = self->msgTimer - 1;
        }
        break;
      }
    }

    if (lost) {
      if (!UiMsgWindowExists()) {
        UiMsgWindowEnsure();
      }
      ((UiMsgWindow *)UiMsgWindowGet())->mode = 1;
      UiMsgWindowOpen((UiMsgWindow *)UiMsgWindowGet(), 0, 0x1c);
      self->base.phaseStep = 7;
      if (NetPlayHasManager()) {
        NetPlaySetState((NetPlay *)NetPlayGetManager(), 4);
        NetPlayClearSelectedHost((NetPlay *)NetPlayGetManager());
      }
    } else if (backToList) {
      if (NetPlayHasManager()) {
        NetPlayClearSelectedHost((NetPlay *)NetPlayGetManager());
      }
      self->base.phaseStep = 9;
      task = CoreTaskFind(1999);
      if (task != NULL) {
        UiNetMenuSetHelpText(task, 3);
      }
    } else {
      UiNetLobbyDrawPlayerList(self, true, false, false, false);
    }
    break;

  case 6: /* wait for the open dialog, then show "lost" */
    ok = true;
    if (UiMsgWindowExists()) {
      ok = false;
      if (UiMsgWindowIsClosed((UiMsgWindow *)UiMsgWindowGet())) {
        ok = true;
      }
    }
    if (ok) {
      if (!UiMsgWindowExists()) {
        UiMsgWindowEnsure();
      }
      ((UiMsgWindow *)UiMsgWindowGet())->mode = 1;
      UiMsgWindowOpen((UiMsgWindow *)UiMsgWindowGet(), 0, 0x1c);
      self->base.phaseStep = 7;
    }
    break;

  case 7: /* "lost" closed: back to the list */
    ok = true;
    if (UiMsgWindowExists()) {
      ok = false;
      if (UiMsgWindowIsClosed((UiMsgWindow *)UiMsgWindowGet())) {
        ok = true;
      }
    }
    if (ok) {
      task = CoreTaskFind(1999);
      if (task != NULL) {
        UiNetMenuSetHelpText(task, 3);
      }
      self->base.phaseStep = 9;
    }
    break;

  case 9:
    if (UiNetLobbyStubDoneB(&self->base, 0) != 0) {
      UiNetLobbyDrawPlayerList(self, false, false, false, false);
      UiNetLobbySetField7c(self, 0x15);
      UiNetLobbyStubDoneA(&self->base, 1);
      self->base.phaseStep = 10;
    }
    break;

  case 10:
    if (UiNetLobbyStubDoneA(&self->base, 0) != 0) {
      UiNetLobbyDrawPlayerList(self, true, true, true, true);
      UiMenuFlagsModify(0, 1);
      self->base.phaseStep = 2;
    }
    break;

  case 0xc: /* "quit?" dialog from the list */
    ok = true;
    confirmed = true;
    if (UiMsgWindowExists()) {
      confirmed = false;
      if (UiMsgWindowIsClosed((UiMsgWindow *)UiMsgWindowGet())) {
        /* choice 0 = yes; -1, 1 and anything else = no */
        if (UiMsgWindowGetChoice((UiMsgWindow *)UiMsgWindowGet()) == 0) {
          confirmed = true;
        }
      } else {
        ok = false;
        if (SaveHasProfile() && SaveProfileHasFlags(SaveGetProfile(), 0x80)) {
          UiMsgWindowRequestClose((UiMsgWindow *)UiMsgWindowGet());
        }
      }
    }
    if (ok) {
      self->base.phaseStep = 2;
      if (confirmed) {
        self->base.phaseStep = 0x10;
        GfxRectSetVisible(self->highlightRect, 0);
        UiNetLobbyStubDoneB(&self->base, 1);
        UiNetLobbyHidePanels(self, 1);
        if (NetPlayHasManager()) {
          NetPlayRequestAbort((NetPlay *)NetPlayGetManager());
        }
      }
    }
    break;

  case 0xd: /* "cancel?" dialog while waiting for the host */
    ok = true;
    confirmed = true;
    if (UiMsgWindowExists()) {
      confirmed = false;
      if (UiMsgWindowIsClosed((UiMsgWindow *)UiMsgWindowGet())) {
        /* choice 0 = yes; -1, 1 and anything else = no */
        if (UiMsgWindowGetChoice((UiMsgWindow *)UiMsgWindowGet()) == 0) {
          confirmed = true;
        }
      } else {
        ok = false;
        if (SaveHasProfile() && SaveProfileHasFlags(SaveGetProfile(), 0x80)) {
          UiMsgWindowRequestClose((UiMsgWindow *)UiMsgWindowGet());
        }
        /* keep watching the host while the dialog is up */
        notFound = true;
        lost = false;
        peer = (NetPlayPeer *)NetPlayGetSelectedHost((NetPlay *)NetPlayGetManager());
        if (peer != NULL) {
          i = 1;
          do {
            if (NetCharaGetRecvHeader(&hdr, i) &&
                hdr.mac[0] == peer->mac[0] && hdr.mac[1] == peer->mac[1] &&
                hdr.mac[2] == peer->mac[2] && hdr.mac[3] == peer->mac[3] &&
                hdr.mac[4] == peer->mac[4] && hdr.mac[5] == peer->mac[5]) {
              self->msgTimer = 150;
              notFound = false;
              if ((hdr.flags & 1) != 0) {
                UiMsgWindowRequestClose((UiMsgWindow *)UiMsgWindowGet());
              }
              break;
            }
            i++;
          } while (i < 16);
        }
        if (notFound) {
          if (self->msgTimer <= 0) {
            lost = true;
          } else {
            self->msgTimer = self->msgTimer - 1;
          }
        } else {
          self->msgTimer = 150;
        }
        if (lost) {
          UiMsgWindowRequestClose((UiMsgWindow *)UiMsgWindowGet());
          self->base.phaseStep = 6;
          if (NetPlayHasManager()) {
            NetPlaySetState((NetPlay *)NetPlayGetManager(), 4);
            NetPlayClearSelectedHost((NetPlay *)NetPlayGetManager());
          }
        }
        if (NetPlayHasManager() && NetPlayGetState((NetPlay *)NetPlayGetManager()) == 6) {
          /* host launched meanwhile */
          UiMsgWindowRequestClose((UiMsgWindow *)UiMsgWindowGet());
          self->base.phaseStep = 0xf;
          GfxRectSetVisible(self->highlightRect, 0);
          UiNetLobbyStubDoneB(&self->base, 1);
          UiNetLobbyHidePanels(self, 1);
        }
      }
    }
    if (ok) {
      self->base.phaseStep = 5;
      if (confirmed) {
        if (NetPlayHasManager()) {
          NetPlaySetState((NetPlay *)NetPlayGetManager(), 4);
          NetPlayClearSelectedHost((NetPlay *)NetPlayGetManager());
        }
        self->base.phaseStep = 9;
        task = CoreTaskFind(1999);
        if (task != NULL) {
          UiNetMenuSetHelpText(task, 3);
        }
      }
    }
    break;

  case 0xe: /* "full" dialog: back to the list once closed */
    ok = true;
    if (UiMsgWindowExists()) {
      ok = false;
      if (UiMsgWindowIsClosed((UiMsgWindow *)UiMsgWindowGet())) {
        ok = true;
      }
    }
    if (ok) {
      self->base.phaseStep = 2;
    }
    break;

  case 0xf: /* host launched: hide, then enter phase 3 once the session runs */
    UiNetLobbyDrawPlayerList(self, false, false, false, false);
    if (UiNetLobbyStubDoneB(&self->base, 0) == 0) {
      ok = false;
    }
    if (UiNetLobbyHidePanels(self, 0) == 0) {
      ok = false;
    }
    if (ok) {
      ok = false;
      if (!NetPlayHasManager()) {
        self->base.phaseStep = 0x10;
        SaveProfileSetFlags(SaveGetProfile(), 0x80);
      } else if (NetPlayGetState((NetPlay *)NetPlayGetManager()) == 7) {
        ok = true;
      }
      if (ok) {
        self->base.phase = 3;
        self->base.phaseStep = 0;
      }
    }
    break;

  case 0x10: /* leave the lobby once NetPlay is gone */
    UiNetLobbyDrawPlayerList(self, true, true, true, false);
    if (UiNetLobbyStubDoneB(&self->base, 0) == 0) {
      ok = false;
    }
    if (UiNetLobbyHidePanels(self, 0) == 0) {
      ok = false;
    }
    if (NetPlayHasManager()) {
      ok = false;
    }
    if (ok) {
      self->base.phase = 3;
      self->leaving = 1;
      self->base.phaseStep = -1;
    }
    break;
  }
}
