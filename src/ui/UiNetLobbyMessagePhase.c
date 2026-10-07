// bdc 0x08941dd8 UiNetLobbyMessagePhase
#include "bdc.h"

/* Phase 3 of `UiNetLobby` (phase table `0x08a9cf78`): if the save profile has flag
   0x80 it clears it and opens message 0xe in the message window (`UiMsgWindowOpen`, creating the
   window if needed); once no message window is open it moves to phase 4. */

void UiNetLobbyMessagePhase(UiNetLobby *self)

{
  bool done = true;

  if (SaveHasProfile() && SaveProfileHasFlags(SaveGetProfile(), 0x80)) {
    SaveProfileClearFlags(SaveGetProfile(), 0x80);
    if (!UiMsgWindowExists()) {
      UiMsgWindowEnsure();
    }
    ((UiMsgWindow *)UiMsgWindowGet())->mode = 1;
    UiMsgWindowOpen((UiMsgWindow *)UiMsgWindowGet(), 0, 0xe);
    return;
  }
  if (UiMsgWindowExists()) {
    if (!UiMsgWindowIsClosed((UiMsgWindow *)UiMsgWindowGet())) {
      done = false;
    }
  }
  if (done) {
    self->base.phase = 4;
    self->base.phaseStep = 0;
  }
}
