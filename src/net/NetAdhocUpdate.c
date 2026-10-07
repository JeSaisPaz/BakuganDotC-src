// bdc 0x089d4ff4 NetAdhocUpdate
#include "bdc.h"

/* Main step of the `COPSPNet` ad-hoc connection object (called by the net thread `BootNetworkThread`):
   first runs `NetAdhocCheckErrors` (WLAN switch check, `NetErrorUpdate`, phase handling), then,
   only while the vblank budget `g_netVblankBudget` is positive (it consumes one), advances the
   connection state `ctlState` (0 idle: waits for a requested phase, 1 `NetAdhocLoadModules`,
   2 `NetAdhocUnloadModules`, 3 loaded: waits for a phase, 4 `NetAdhocInitStep`, 5 `NetAdhocTermStep`,
   6 connected) and resets the state step `step` on each change. Errors go to `NetAdhocPostError`;
   reaching state 6 records the role (`linkedRole` 1 for phase 3, 2 for phase 4), resets the phase
   (`NetAdhocSetPhase` 0) and starts the link (`NetAdhocSetLinkState(self, 1)`). In state 6 a phase
   request that does not match the linked role drops back to state 5 (without resetting `step`);
   otherwise the link is run (`NetAdhocUpdateLink`). */

void NetAdhocUpdate(NetAdhocConn *self)
{
  s32 result;
  s32 phase;
  bool lost;

  NetAdhocCheckErrors(self);
  if (g_netVblankBudget < 1) {
    return;
  }
  g_netVblankBudget = g_netVblankBudget - 1;

  switch (self->ctlState) {
  case 0:
    phase = NetAdhocGetPhase(self);
    if ((u32)(phase - 1) < 5) {
      if (phase == 2 || phase == 5) {
        NetAdhocSetPhase(self, 0);
      } else {
        self->ctlState = 1;
        self->step = 0;
      }
    }
    break;

  case 1:
    result = NetAdhocLoadModules(self);
    if (result > 0) {
      self->ctlState = 3;
      self->step = 0;
    } else if (result < 0) {
      NetAdhocPostError(self, result);
      self->ctlState = 2;
      self->step = 0;
    }
    break;

  case 2:
    result = NetAdhocUnloadModules(self);
    if (result > 0) {
      self->ctlState = 0;
      self->step = 0;
    } else if (result < 0) {
      NetAdhocPostError(self, result);
    }
    break;

  case 3:
    phase = NetAdhocGetPhase(self);
    switch (phase) {
    case 2:
      self->ctlState = 2;
      self->step = 0;
      break;
    case 3:
    case 4:
    case 6:
      self->ctlState = 4;
      self->step = 0;
      break;
    case 5:
      NetAdhocSetPhase(self, 0);
      break;
    default:
      break;
    }
    break;

  case 4:
    result = NetAdhocInitStep(self);
    if (NetAdhocGetPhase(self) == 5) {
      self->ctlState = 5;
      self->step = 0;
    } else if (result > 0) {
      phase = NetAdhocGetPhase(self);
      if (phase == 3) {
        self->linkedRole = 1;
      } else if (phase == 4) {
        self->linkedRole = 2;
      }
      NetAdhocSetPhase(self, 0);
      NetAdhocSetLinkState(self, 1);
      self->phaseActive = 0;
      self->ctlState = 6;
      self->step = 0;
    } else if (result < 0) {
      NetAdhocPostError(self, result);
      self->ctlState = 5;
      self->step = 0;
    }
    break;

  case 5:
    result = NetAdhocTermStep(self);
    if (result > 0) {
      phase = NetAdhocGetPhase(self);
      if (phase == 3 || phase == 4 || phase == 6) {
        self->ctlState = 4;
      } else {
        NetAdhocSetPhase(self, 0);
        self->linkedRole = 0;
        self->ctlState = 3;
      }
      NetAdhocSetLinkState(self, 0);
      self->step = 0;
    } else if (result < 0) {
      NetAdhocPostError(self, result);
    }
    break;

  case 6:
    lost = false;
    phase = NetAdhocGetPhase(self);
    if (phase == 3) {
      if (self->linkedRole == 1) {
        NetAdhocSetPhase(self, 0);
      } else {
        lost = true;
      }
    } else if (phase == 4) {
      if (self->linkedRole == 2) {
        NetAdhocSetPhase(self, 0);
      } else {
        lost = true;
      }
    } else if (phase != 0) {
      lost = true;
    }
    if (lost) {
      self->ctlState = 5;
    } else {
      NetAdhocUpdateLink(self);
    }
    break;

  default:
    break;
  }
}
