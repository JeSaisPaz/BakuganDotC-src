// bdc 0x089d3e28 NetAdhocRequestConnect
#include "bdc.h"

/* Requests an ad-hoc connection: when no net error is pending (`NetAdhocHasError`) and the phase
   is 0, stores `mode` (`+0x24`: 1 lobby, 2 game) and `role` (`+0x28`: 0 scan, 1 host, 2 client) and
   sets phase 3 (mode 1) or 4 (mode 2). Returns 1 when accepted, 0 when busy. */

bool NetAdhocRequestConnect(NetAdhocConn *self, s32 mode, s32 role)

{
  bool accepted = false;

  if (!NetAdhocHasError() && NetAdhocPhaseIs(self, 0)) {
    self->mode = mode;
    self->role = role;
    if (mode < 2) {
      accepted = true;
      if (0 < mode) {
        NetAdhocSetPhase(self, 3);
      }
    }
    else {
      accepted = true;
      if (mode < 3) {
        NetAdhocSetPhase(self, 4);
      }
    }
  }
  return accepted;
}

