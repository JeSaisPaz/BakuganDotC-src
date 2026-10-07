// bdc 0x089d2b50 NetInviteClearPendingA
#include "bdc.h"

/* Clears the invite object's first pending flag (byte `+4`) under its lock. `NetCharaUpdate`
   calls it when the peer named in that invite timed out. */

void NetInviteClearPendingA(NetInvite *self)

{
  CoreLockAcquire(self->lock);
  self->pendingA = '\0';
  CoreLockRelease(self->lock);
  return;
}

