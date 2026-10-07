// bdc 0x089d3f7c NetAdhocIsDisconnected
#include "bdc.h"

/* Returns 1 when the connection is idle: phase 0, link state 0 and connection state `+0x4c` 0 or 3
   (read under the lock). The NetPlay lobby/session states treat a 1 here as "link lost". */

bool NetAdhocIsDisconnected(NetAdhocConn *self)
{
  int state;
  bool result;

  result = false;
  if (NetAdhocPhaseIs(self, 0) && NetAdhocLinkStateIs(self, 0)) {
    CoreLockAcquire(self->lock);
    state = self->ctlState;
    if (state < 1) {
      if (-1 < state) {
        result = true;
      }
    } else if (state == 3) {
      result = true;
    }
    CoreLockRelease(self->lock);
  }
  return result;
}
