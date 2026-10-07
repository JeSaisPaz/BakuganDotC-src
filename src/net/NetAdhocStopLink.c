// bdc 0x089d4054 NetAdhocStopLink
#include "bdc.h"

/* Step 2 of `NetPlayStateAbort`: sets the stop byte `+0x1e`; if the link is down and the state
   `+0x4c` is 0 it returns 1, in state 3 it sets phase 2 and returns 1, otherwise (still linked or
   another state) it requests phase 5 (stop) and returns 0 so the caller retries. */

bool NetAdhocStopLink(NetAdhocConn *self)

{
  s32 ctlState;

  CoreLockAcquire(self->lock);
  ctlState = self->ctlState;
  self->stopRequested = '\x01';
  CoreLockRelease(self->lock);
  if (!NetAdhocLinkStateIs(self, 1)) {
    if (ctlState < 1) {
      if (-1 < ctlState) {
        return true;
      }
    }
    else if (ctlState == 3) {
      NetAdhocSetPhase(self,2);
      return true;
    }
    NetAdhocSetPhase(self,5);
  }
  else {
    NetAdhocSetPhase(self,5);
  }
  return false;
}

