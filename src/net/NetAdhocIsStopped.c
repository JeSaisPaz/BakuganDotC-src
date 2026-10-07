// bdc 0x089d4114 NetAdhocIsStopped
#include "bdc.h"

/* Returns 1 when a stop was requested (`+0x1e`), the link state is 0 and the connection state
   `+0x4c` is back at 0. Step 3 of `NetPlayStateAbort`; the net thread `BootNetworkThread` exits on it.
    */

bool NetAdhocIsStopped(NetAdhocConn *self)
{
  bool stopped;

  CoreLockAcquire(self->lock);
  stopped = false;
  if (self->stopRequested != 0) {
    stopped = false;
    if (NetAdhocLinkStateIs(self, 0) && self->ctlState == 0) {
      stopped = true;
    }
  }
  CoreLockRelease(self->lock);
  return stopped;
}
