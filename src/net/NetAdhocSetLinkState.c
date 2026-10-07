// bdc 0x089d49d8 NetAdhocSetLinkState
#include "bdc.h"

/* Stores the link state `+0x14` of the connection object under its lock; `NetAdhocUpdate` sets 1
   on reaching the connected state, `NetAdhocConnCtor` 0. */

void NetAdhocSetLinkState(NetAdhocConn *self, s32 state)

{
  CoreLockAcquire(self->lock);
  self->linkState = state;
  CoreLockRelease(self->lock);
  return;
}

