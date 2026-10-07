// bdc 0x089d4980 NetAdhocLinkStateIs
#include "bdc.h"

/* Returns whether the connection's link state `+0x14` equals `state` (under the lock); 1 = linked.
    */

bool NetAdhocLinkStateIs(NetAdhocConn *self, s32 state)

{
  s32 cur;
  
  CoreLockAcquire(self->lock);
  cur = self->linkState;
  CoreLockRelease(self->lock);
  return cur == state;
}

